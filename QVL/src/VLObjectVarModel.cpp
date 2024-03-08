#include <unordered_set>
#include <QObject>
#include <QVariant>
#include <utils/log.h>
#ifdef LOG_ON
    #include <QDebug>
#endif
#include <utils/string_utils.h>
#include "VLObjectVarModel.h"
#include "VLCollectionModel.h"
#include "VLVarModel.h"
#include "vl.h"
#include "DMBModel.h"
LOG_TITLE("VLCollectionModel")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(verbose)

namespace
{
	dmb::VLVarModelPtr nullVarModelPtr = nullptr;
}

namespace dmb
{
	const std::string emptyString;

	VLObjectVarModel::VLObjectVarModel(QObject *parent)
		: Base(parent)
		, mAllPropsListModel(std::make_unique<AllPropsCollectionModel>(this, std::make_shared<ObjectModelStorage>()))
		, mOwnPropsListModel(std::make_unique<OwnPropsCollectionModel>(this, *mAllPropsListModel)) // Share the same storage
	{
		if (parent) {
			LOCAL_VERBOSE("Object(QObject* parent) Object with a parent created");
		} else {
			LOCAL_VERBOSE("Object(QObject* parent == nullptr) Empty object cretaed");
		}
	}

	VLObjectVarModel::VLObjectVarModel(const vl::Var& v, QObject* parent)
		: Base(v, parent)
		, mAllPropsListModel(std::make_unique<AllPropsCollectionModel>(this, std::make_shared<ObjectModelStorage>()))
		, mOwnPropsListModel(std::make_unique<OwnPropsCollectionModel>(this, *mAllPropsListModel)) // Share the same storage
	{}

	VLObjectVarModel::~VLObjectVarModel() {
		LOCAL_DEBUG("~Object() (" << this << ")");
		emit beforeRemove();
		mOwnPropsListModel.reset();
		mAllPropsListModel.reset();
	}

	bool VLObjectVarModel::loadPropList()
	{
		return getOwnPropsListModel().loadElementModels();
	}

	void VLObjectVarModel::Init(QObject *parent)
	{
		Base::Init(parent);
		getOwnPropsListModel().setParent(this);
		LOCAL_DEBUG(&getOwnPropsListModel() << "->(collection)setParent(" << this << ")");
		getAllPropsListModel().setParent(this);
		LOCAL_DEBUG(&getAllPropsListModel() << "->(collection)setParent(" << this << ")");
	}

	void VLObjectVarModel::Init(QObject *parent, DMBModel* owner)
	{
		Base::Init(parent, owner);
		auto ptr = shared_from_this();
		getAllPropsListModel().Init(ptr);
		getOwnPropsListModel().Init(ptr);
		// We have the data object to update the id list
		getAllPropsListModel().UpdateIdList();
		// Uncomment if there are problems with not existent nested models
		// Only in the first call of the nested model
		//loadPropList();
	}

	void VLObjectVarModel::Init(QObject *parent, const vl::Var &data, DMBModel *owner)
	{
		Base::Init(parent, data, owner);
	}

	void VLObjectVarModel::Init(const VLVarModelPtr& parent)
	{
		Base::Init(parent);
		auto ptr = shared_from_this();
		getOwnPropsListModel().Init(ptr);
		getAllPropsListModel().Init(ptr);
		// The object is supposed to be registered
		// in the parent's storage
		getAllPropsListModel().UpdateIdList();
	}

	// Type casts
	const VLObjectVarModel *VLObjectVarModel::asObject() const
	{
		return this;
	}

	VLObjectVarModel *VLObjectVarModel::asObject()
	{
		return this;
	}

	// Type check
	bool VLObjectVarModel::isObject() const
	{
		return true;
	}

	// Begin of INVOKABLE helpful list-like interface
	QVariant VLObjectVarModel::at(int index)
	{
		return getOwnPropsListModel().at(index);
	}

	bool VLObjectVarModel::removeAt(int index)
	{
		return getOwnPropsListModel().removeAt(index);
	}
	// End of helpful INVOKABLE list-like interface

	// Redirects to has(const std::string)
	// Check the data recursively through the proto chain
	bool VLObjectVarModel::has(const QString &propId) const
	{
		return has(propId.toStdString());
	}

	// Redirects to hasOwn(const std::string)
	// Checks the data just in its object
	bool VLObjectVarModel::hasOwn(const QString &propId) const
	{
		return hasOwn(propId.toStdString());
	}

	// Checks the data recursively through the proto chain
	bool VLObjectVarModel::has(const std::string &propId) const
	{
		return getData().Has(propId);
	}

	// Checks the data just in its object
	bool VLObjectVarModel::hasOwn(const std::string &propId) const
	{
		return getData().HasOwn(propId);
	}

	const vl::ObjectVar &VLObjectVarModel::getData() const
	{
		return Base::getData().AsObject();
	}

	vl::ObjectVar &VLObjectVarModel::data()
	{
		return const_cast<vl::ObjectVar&>(const_cast<const VLObjectVarModel*>(this)->getData());
	}

	const VLVarModelPtr &VLObjectVarModel::getChild(const VLVarModel *p) const
	{
		if (auto id = getChildId(p))
			return getModel(*id);
		return nullVarModelPtr;
	}

	// Redirects to the propListModel
	// Removes data and model from the parent
	bool VLObjectVarModel::removeChild(const VLVarModel *childPtr)
	{
		return getOwnPropsListModel().removeElement(childPtr);
	}

	bool VLObjectVarModel::renameProperty(const std::string &propId, const std::string &newId)
	{
		return getOwnPropsListModel().renameElement(propId, newId);
	}

	const vl::Var &VLObjectVarModel::getChildData(const VLVarModel *childPtr) const
	{
		if (auto id = getChildId(childPtr))
			return getData().Get(*id);
		return vl::emptyVar;
	}

	// Begin of Model retrieving by id interface
	// Redirects to propListModel
	// Return null if the model doesn't exist
	const VLVarModelPtr& VLObjectVarModel::getModel(const std::string &propId) const
	{
		return getAllPropsListModel().getModel(propId);
	}

	// Redirects to propListModel
	// Creates a model if doesn't exist
	const VLVarModelPtr& VLObjectVarModel::model(const std::string &propId)
	{
		return getAllPropsListModel().model(propId);
	}

	// Redirects to propListModel
	const VLVarModelPtr &VLObjectVarModel::setModel(const std::string &propId, const VLVarModelPtr &modelPtr)
	{
		// Should call it on AllPropsCollectionModel to be able
		// to check the existent shared model and restrict adding a new model if it exists
		return getAllPropsListModel().setModel(propId, modelPtr);
	}

	const std::string *VLObjectVarModel::getChildId(const VLVarModel *childPtr) const
	{
		return getOwnPropsListModel().getElementId(childPtr);
	}

	// Redirects to allPropListModel
	// Creates a model if doesn't exist
	// Return its own prop even if it is of a proto own
	QVariant VLObjectVarModel::get(const QString &propId)
	{
		return getAllPropsListModel().get(propId);
	}

	QVariant VLObjectVarModel::getOwn(const QString &propId)
	{
		return getOwnPropsListModel().get(propId);
	}
	// End of Model retrieving by id interface

	VLCollectionModel* VLObjectVarModel::propListModel()
	{
		if (!getOwnPropsListModel().elementsLoaded())
			loadPropList();
		return &getOwnPropsListModel();
	}

	QVariant VLObjectVarModel::protoPropListModel()
	{
		if (auto& protoModel = model("proto"))
			if (auto objectModel = protoModel->asObject())
				return QVariant::fromValue(objectModel->propListModel());
		return QVariant();
	}

	QVariant VLObjectVarModel::allPropListModel()
	{
		if (!getAllPropsListModel().elementsLoaded())
		{
			getAllPropsListModel().UpdateIdList();
			getAllPropsListModel().loadElementModels();
		}
		return QVariant::fromValue(&getAllPropsListModel());
	}

	void VLObjectVarModel::add(const QString& propId, ObjectProperty::Type type)
	{
		getOwnPropsListModel().add(propId, type);
	}

	bool VLObjectVarModel::removeProp(const QString& propName)
	{
		return getOwnPropsListModel().remove(propName);
	}

	// Redirects to propListModel
	bool VLObjectVarModel::setChildType(const VLVarModel *child, const ObjectProperty::Type &newType)
	{
		return getOwnPropsListModel().setElementType(child, newType);
	}

	const vl::Var &VLObjectVarModel::setChildValue(const VLVarModel *child, const vl::VarPtr &value)
	{
		return getOwnPropsListModel().setElementValue(child, value);
	}

	QVariant VLObjectVarModel::set(const QString &propId, const QVariant &data)
	{
		return getOwnPropsListModel().set(propId, data);
	}

	std::string VLObjectVarModel::getFreeId(const std::string& desiredId) const
	{
		return getOwnPropsListModel().getFreeId(desiredId);
	}

	QString VLObjectVarModel::freeId(const QString& desiredId) const
	{
		return QString(getFreeId(desiredId.toStdString()).c_str());
	}

	void VLObjectVarModel::instantiate(const QString& typeId, const QString& instanceName)
	{
		if (auto owner = getDataModel())
		{
			if (instanceName.isEmpty())
			{
				auto instId = getFreeId(utils::format_str("Concrete_%s", typeId.toStdString().c_str()));
				if (!instId.empty())
					emit instantiateRequested(QString(instId.c_str()), typeId);
				else
					emit instantiateRefused("Name error");
			}
			else
			{
				auto instId = instanceName.toStdString();
				if (hasOwn(instId))
					emit instantiateRefused("Name error");
				else
				{
					auto o = owner->createObjectSp();
					if (o->setPrototype(typeId))
					{
						setModel(instId, o);
						emit instantiated(typeId, instanceName);
					}
					else
						emit instantiateRefused("Type error");
				}
			}
		}
	}

	bool VLObjectVarModel::setPrototype(const QVariant& arg)
	{
		if (arg.canConvert<VLObjectVarModel*>())
		{
			if (auto model = qvariant_cast<VLObjectVarModel*>(arg))
				if (auto modelPtr = model->shared_from_this())
					return setModel("proto", modelPtr) != nullptr;
		}
		else if (arg.userType() == QMetaType::QString)
			return setPrototype(arg.toString().toStdString());
		return false;
	}

	bool VLObjectVarModel::setPrototype(const std::string &protoId)
	{
		if (auto owner = getDataModel())
			if (auto modelPtr = owner->modelByTypeId(protoId))
				return setModel("proto", modelPtr) != nullptr;
		return false;
	}

	int VLObjectVarModel::sizeOwn() const
	{
		return getData().Size();
	}

	int VLObjectVarModel::sizeAll() const
	{
		int i = 0;
		getData().ForeachProp(
			[&](const std::string& propName, const vl::Var& prop) {
				i++;
				return true;
			}, true
		);
		return i;
	}

	QVariant VLObjectVarModel::protoId() const
	{
		if (auto& protoModel = getModel("proto"))
			return protoModel->asObject()->typeId();
		return QVariant();
	}

	QVariant VLObjectVarModel::typeId() const
	{
		return QVariant(QString(getTypeId().c_str()));
	}
}
