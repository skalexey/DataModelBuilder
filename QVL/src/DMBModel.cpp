#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include "DMBModel.h"
#include "DMBCore.h"
#include "Utils.h"
#include "VLObjectVarModel.h"
#include "VLListVarModel.h"
#include "VarModelFactory.h"

namespace
{
	const dmb::VLVarModelPtr emptyVarModelPtr = nullptr;

	std::string getAbsolutePath(const QString& filePath)
	{
		QUrl u(filePath);
		std::string fPath;
		if (u.scheme() == "file")
		{
			if (u.isRelative())
			{
				auto p = u.path();
				QDir currentDir = QDir::currentPath();
				fPath = currentDir.absoluteFilePath(p).toStdString();
			}
			else
				fPath = u.toLocalFile().toStdString();
		}
		else
		{
			QFileInfo f(filePath);
			if (f.isRelative())
			{
				QDir currentDir = QDir::currentPath();
				fPath = currentDir.absoluteFilePath(filePath).toStdString();
			}
			else
				fPath = filePath.toStdString();
		}
		return fPath;
	}
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
		auto fPath = getAbsolutePath(filePath);
		if (!filePath.isEmpty())
			setCurrentFile(fPath);
		if (getDataModel().Store(mFilePath, { pretty }))
		{
			emit modelStored(QString(mFilePath.c_str()));
			return true;
		}
		else
		{
			emit modelStoreError(QString(mFilePath.c_str()), Utils::FormatStr("Can't store the model to the file '%s'", fPath.c_str()).c_str());
			return false;
		}
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
		auto fPath = getAbsolutePath(url);
		if (getDataModel().Load(fPath))
		{
			if (!firstLoad)
				// The list of ids created in Init method
				// on the object creation or loaded here
				mRoot->getPropListModel().UpdateIdList();
			// Create new Qt models for types and content
			mRoot->loadPropList();
			setCurrentFile(fPath.c_str());
			emit modelLoaded(QString(fPath.c_str()));
			return true;
		}
		else
		{
			emit modelLoadError(url, Utils::FormatStr("Can't load the file '%s'", fPath.c_str()).c_str());
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

	VLObjectVarModel *DMBModel::createObject()
	{
		return createObjectSp().get();
	}

	VLListVarModel *DMBModel::createList()
	{
		return createListSp().get();
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

	VLObjectVarModelPtr DMBModel::createObjectSp()
	{
		auto ptr = VarModelFactory::Instance().CreateObject();
		if (ptr)
			if (storeStandalonePtr(ptr))
				return ptr;
		return nullptr;
	}

	VLListVarModelPtr DMBModel::createListSp()
	{
		auto ptr = VarModelFactory::Instance().CreateList();
		if (ptr)
			if (storeStandalonePtr(ptr))
				return ptr;
		return nullptr;
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

	bool DMBModel::setLoadFrom(const QString &fPath)
	{
		return load(fPath);
	}
}
