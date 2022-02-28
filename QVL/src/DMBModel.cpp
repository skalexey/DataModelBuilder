#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include "DMBModel.h"
#include "DMBCore.h"
#include "Utils.h"
#include "VLObjectVarModel.h"
#include "VLListVarModel.h"
#include "VarModelFactory.h"
#include "Log.h"

namespace
{
	const dmb::VLVarModelPtr emptyVarModelPtr = nullptr;

	std::string getAbsolutePath(const QString& filePath)
	{
		QUrl u(filePath);
		std::string fPath;
		if (u.scheme() == "file" || u.scheme() == "qrc")
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
		if (auto model = mRoot->model("content"))
			return model->asObject();
		return nullptr;
	}

	const VLObjectVarModel *DMBModel::getTypesModel() const
	{
		// Invoke const version of getModel
		if (auto model = mRoot->model("types"))
			return model->asObject();
		return nullptr;
	}

	VLObjectVarModel *DMBModel::typesModel()
	{
		// Invoke non-const version of getModel
		if (auto model = mRoot->model("types"))
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

	bool DMBModel::store(const QString &filePath, bool pretty)
	{
		auto fPath = getAbsolutePath(filePath);
		if (!filePath.isEmpty())
			setCurrentFile(fPath);
		if (mFilePath.empty())
		{
			emit modelStoreError(QString(""), QString("Empty file path"));
			return false;
		}
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

	bool DMBModel::storeContent(const QString &filePath, bool pretty)
	{
		auto fPath = getAbsolutePath(filePath);
		if (fPath.empty())
		{
			emit modelStoreError(QString(""), QString("Empty file path"));
			return false;
		}
		if (getDataModel().GetContent().Store(fPath, { pretty }))
		{
			emit modelStored(QString(fPath.c_str()));
			return true;
		}
		else
		{
			emit modelStoreError(QString(fPath.c_str()), Utils::FormatStr("Can't store the content to the file '%s'", fPath.c_str()).c_str());
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

	const VLVarModelPtr &DMBModel::storeStandaloneModel(const VLVarModelPtr &ptr)
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
			return storeStandaloneModel(ptr).get();
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

	void DMBModel::clear()
	{
		mRoot->asObject()->getPropListModel().foreachElement([&](int i, const VLVarModelPtr& m) {
			VLVarModel* mp = m.get();
			if (auto l = mp->asList())
				l->getListModel().clearAndNotify();
			else if (auto o = m->asObject())
				o->getPropListModel().clearAndNotify();
			return true;
		}, true);
		mDataModel.Clear();
		setCurrentFile("");
	}

	bool DMBModel::hasChanges() const
	{
		if (mFilePath.empty())
			return false;
		dmb::Model m;
		m.Load(mFilePath);
		return m.DataStr(false) != mDataModel.DataStr(false);
	}

	VLVarModelPtr DMBModel::createPtrFromData(const QVariant &data)
	{
		vl::VarPtr v = ObjectProperty::makeVarFromData(data);
		return VarModelFactory::Instance().Create(*v);
	}

	VLObjectVarModelPtr DMBModel::createObjectSp()
	{
		auto ptr = VarModelFactory::Instance().CreateObject();
		if (ptr)
			if (storeStandaloneModel(ptr))
				return ptr;
		return nullptr;
	}

	VLListVarModelPtr DMBModel::createListSp()
	{
		auto ptr = VarModelFactory::Instance().CreateList();
		if (ptr)
			if (storeStandaloneModel(ptr))
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

	const VLVarModelPtr &DMBModel::modelByTypeId(const std::string &path)
	{
		// Find in types if this is not a path but just a token
		if (path.find_first_of('.') == std::string::npos)
			if (auto types = typesModel())
				if (auto& m = types->modelSp(path))
					return m;

		// Parse the path
		auto parseIndex = [](const std::string& id) {
			if (id.size() < 3)
				return -1;
			return std::atoi(id.substr(1, id.size() - 2).c_str());
		};
		auto dotPos = std::string::npos;
		size_t cursor = 0;
		std::string lastId;
		const VLVarModelPtr* node = &nullVarModelPtr;
		auto root = std::dynamic_pointer_cast<VLVarModel>(mRoot);
		do {
			dotPos = path.find_first_of('.', cursor);
			auto id = dotPos == std::string::npos ?
				path.substr(cursor) : path.substr(cursor, dotPos - cursor);
			auto n = ((node && *node) ? node : &root)->get();
			if (auto o = n->asObject())
				node = &o->modelSp(id);
			else if (auto l = n->asList())
			{
				auto index = parseIndex(id);
				if (index >= 0 && index < l->getData().Size())
					node = &l->atSp(index);
				else
				{
					qDebug() << Utils::FormatStr("QVL: Wrong index %d in the path '%s' used for node '%s'", index, path.c_str(), lastId.c_str()).c_str();
					return nullVarModelPtr;
				}
			}
			else
			{
				qDebug() << Utils::FormatStr("QVL: Wrong container type during parsing the node path '%s'", path.c_str()).c_str();
				return nullVarModelPtr;
			}
			cursor = dotPos + 1;
			lastId = id;

		} while (dotPos != std::string::npos && node != nullptr);
		return *node;
	}
}
