#include <QUrl>
#include "DMBModel.h"
#include "DMBCore.h"
#include "Utils.h"
#include "VLObjectVarModel.h"
#include "VarModelFactory.h"

namespace
{
	const dmb::VLVarModelPtr emptyVarModelPtr = nullptr;
}

namespace dmb
{
	DMBModel::DMBModel(QObject *parent)
		: QObject(parent)
	{
		mRoot = std::make_shared<VLObjectVarModel>();
		mRoot->Init(nullptr, getDataModel().GetData(), this);
	}

	DMBModel::~DMBModel()
	{}

	const VLObjectVarModel *DMBModel::getContentModel() const
	{
		// Invoke const version of getModel
		if (auto model = mRoot->getModel("content"))
			return model->asObject();
		return nullptr;
	}

	VLObjectVarModel *DMBModel::contentModel()
	{
		// Invoke non-const version of getModel
		if (auto model = mRoot->getModel("content"))
			return model->asObject();
		return nullptr;
	}

	const VLObjectVarModel *DMBModel::getTypesModel() const
	{
		// Invoke const version of getModel
		if (auto model = mRoot->getModel("types"))
			return model->asObject();
		return nullptr;
	}

	VLObjectVarModel *DMBModel::typesModel()
	{
		// Invoke non-const version of getModel
		if (auto model = mRoot->getModel("types"))
			return model->asObject();
		return nullptr;
	}

	const Model &DMBModel::getDataModel() const
	{
		return mDataModel;
	}

	Model &DMBModel::getDataModel()
	{
		return mDataModel;
	}

	void DMBModel::instantiateRequest(int index)
	{
		if (auto m = typesModel())
			if (index >= 0 && index < m->propListModel()->size())
			{
				const auto& id = m->getId(index);
				auto instId = id;
				auto& content = getDataModel().GetContent();
				bool success = true;
				if (content.Has(instId))
				{
					std::string tpl = instId;
					for (int i = 1; i < std::numeric_limits<short>::max(); i++)
						if ((success = !content.Has(instId = Utils::FormatStr("%s %d", tpl.c_str(), i))))
							break;
				}
				if (success)
					emit instantiateRequested(QString(instId.c_str()), QString(id.c_str()));
				else
					emit instantiateRefused("Name error");
			}
	}

	VLVarModel* DMBModel::addToContent(const QString &instId, const QString &qProtoId)
	{
		if (auto m = contentModel())
		{
			auto protoId = qProtoId.toStdString();
			if (auto proto = const_cast<const DMBModel*>(this)->
					getTypesModel()->getData().Get(protoId).AsObject())
			{
				vl::Object o;
				o.SetPrototype(proto);
				m->propListModel()->setData(instId.toStdString(), o);
				if (auto newModel = m->get(instId))
					return newModel;
			}
		}
		return nullptr;
	}

	bool DMBModel::store(const QString &filePath, bool pretty)
	{
		if (!filePath.isEmpty())
			setCurrentFile(filePath.toStdString());
		return getDataModel().Store(mFilePath, { pretty });
	}

	bool DMBModel::load(const QString &url)
	{
		auto& m = mRoot->getPropListModel();
		bool firstLoad = true;
		if (m.elementsLoaded())
		{
			m.clear();
			firstLoad = false;
		}
		QUrl u(url);
		auto fPath = u.isRelative() ? url.toStdString() : u.toLocalFile().toStdString();
		if (getDataModel().Load(fPath))
		{
			if (!firstLoad)
				// The list of ids created in Init method
				// on the object creation or loaded here
				mRoot->getPropListModel().UpdateIdList();
			// Create new Qt models for types and content
			mRoot->loadPropList();
			setCurrentFile(fPath);
			emit modelLoaded(url);
			return true;
		}
		else
		{
			emit modelLoadError(Utils::FormatStr("Can't load the file '%s'", fPath.c_str()).c_str());
		}
		return false;
	}

	bool DMBModel::isLoaded() const
	{
		return mDataModel.IsLoaded();
	}

	const VLVarModelPtr &DMBModel::storeStandalonePtr(const VLVarModelPtr &ptr)
	{
		auto it = mStandaloneModels.emplace(ptr.get(), ptr);
		ptr->Init(this, const_cast<const VLVarModel&>(*ptr).getData(), this);
		return it.first->second;
	}

	void DMBModel::setCurrentFile(const std::string &newFilePath)
	{
		mFilePath = newFilePath;
		emit currentFileChanged();
	}

	VLVarModel *DMBModel::createFromData(const QVariant& data)
	{
		if (auto ptr = createPtrFromData(data))
			return storeStandalonePtr(ptr).get();
		return nullptr;
	}

	VLVarModel *DMBModel::createObject()
	{
		if (auto ptr = createObjectPtr())
			return storeStandalonePtr(ptr).get();
		return nullptr;
	}

	VLVarModel *DMBModel::createList()
	{
		if (auto ptr = createListPtr())
			return storeStandalonePtr(ptr).get();
		return nullptr;
	}

	VLVarModelPtr DMBModel::createPtrFromData(const QVariant &data)
	{
		vl::VarPtr v;
		switch (data.userType())
		{
		case QMetaType::QString:
			v = vl::MakePtr(data.toString().toStdString());
			break;
		case QMetaType::Int:
			v = vl::MakePtr(data.toInt());
			break;
		case QMetaType::Bool:
			v = vl::MakePtr(data.toBool());
		default:
			v = vl::MakePtr(data.toString().toStdString());
		}
		return VarModelFactory::Instance().Create(*v);
	}

	VLVarModelPtr DMBModel::createObjectPtr()
	{
		vl::Object v;
		auto m = VarModelFactory::Instance().Create(v);
		return m;
	}

	VLVarModelPtr DMBModel::createListPtr()
	{
		vl::List v;
		auto m = VarModelFactory::Instance().Create(v);
		return m;
	}

	VLVarModelPtr DMBModel::takeStandaloneModel(const VLVarModel *p)
	{
		auto it = mStandaloneModels.find(p);
		if (it != mStandaloneModels.end())
		{
			auto ptr = it->second;
			mStandaloneModels.erase(it);
			return ptr;
		}
		return emptyVarModelPtr;
	}

	bool DMBModel::removeStandaloneModel(const VLVarModel *p)
	{
		return mStandaloneModels.erase(p) > 0;
	}

	QString DMBModel::currentFile() const
	{
		return QString(mFilePath.c_str());
	}

	const std::string &DMBModel::getCurrentFile() const
	{
		return mFilePath;
	}
}
