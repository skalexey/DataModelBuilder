#include "VLListModel.h"
#include "VLListVarModel.h"
#include "DMBModel.h"
#include "VarModelFactory.h"
#include "VLVarModel.h"

#include "Log.h"
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("VLListModel")
LOG_STREAM([]{return qDebug();})
SET_LOG_DEBUG(true)

namespace
{
	dmb::VLVarModelPtr nullVarModelPtr = nullptr;
}

namespace dmb
{
	VLListModel::VLListModel(QObject* parent)
		: Base(parent)
	{}

	VLListModel::VLListModel(QObject *parent, const ListModelStoragePtr& storage)
		: Base(parent, storage)
	{}

	VLListModel::VLListModel(QObject *parent, const VLListModelInterface &storageOwner)
		: Base(parent, storageOwner)
	{}

	VLListModel::~VLListModel()
	{
		qDebug() << "~VLListModel()";
		qDebug() << this << "->(list)setParent(nullptr)";
	}

	VLListVarModelPtr VLListModel::getParentModel() const
	{
		if (auto parentModel = Base::getParentModel())
			return std::dynamic_pointer_cast<VLListVarModel>(parentModel);
		return nullptr;
	}

	int VLListModel::dataSize() const
	{
		if (const auto* parentModel = getParentModel().get())
			return parentModel->getData().Size();
		return 0;
	}

	// ======= Begin of VLListModelInterface interface =======
	const vl::List &VLListModel::getData() const
	{
		if (const auto* parentModel = getParentModel().get())
			return parentModel->getData();
		return vl::emptyList;
	}

	vl::List &VLListModel::getData()
	{
		return const_cast<vl::List&>(const_cast<const VLListModel*>(this)->getData());
	}

	const vl::Var &VLListModel::getData(int index) const
	{
		return getData().At(index);
	}

	vl::Var &VLListModel::getData(int index)
	{
		return const_cast<vl::Var&>(const_cast<const VLListModel*>(this)->getData(index));
	}

	bool VLListModel::doRemove(int index)
	{
		return getData().Remove(index);
	}

	const VLVarModelPtr& VLListModel::modelAt(int index)
	{
		if (index < 0)
			return nullVarModelPtr;
		else if (index >= size())
			return loadElementModel(index);
		else if (auto& ptr = getListStorage().at(index))
			return ptr;
		else
			return loadElementModel(index);
	}

	// Redirects to the storage.
	// Retrieve a shared pointer to a model at index
	const VLVarModelPtr& VLListModel::getModelAt(int index) const
	{
		if (index < 0 || index >= size())
			return nullVarModelPtr;
		return getStorage()[index];
	}
	// ======= End of VLListModelInterface interface =======

	QHash<int,QByteArray> VLListModel::roleNames() const
	{
		QHash<int, QByteArray> roles = Base::roleNames();
		return roles;
	}

	// Begin of Data and model Setters
	QVariant VLListModel::add(ObjectProperty::Type type, int indexBefore)
	{
		auto dataPtr = ObjectProperty::MakeVarPtr(type);
		if (auto ret = addDataAndLoadModel(dataPtr, indexBefore))
			return QVariant::fromValue(ret.data.get());
		return QVariant();
	}

	QVariant VLListModel::add(const QVariant &data, int indexBefore)
	{
		if (data.canConvert<VLVarModel*>())
			return add(qvariant_cast<VLVarModel*>(data), indexBefore);
		if (auto parent = getParentModel())
			if (auto dataModel = parent->getDataModel())
				return add(dataModel->createFromData(data), indexBefore);
		return QVariant();
	}

	QVariant VLListModel::add(VLVarModel *m, int indexBefore)
	{
		if (!m)
		{
			LOG_ERROR("Null model passed to add('" << m << ")");
			return QVariant();
		}
		return QVariant::fromValue(addModel(m->shared_from_this(), indexBefore).get());
	}

	const VLVarModelPtr& VLListModel::addModel(const VLVarModelPtr& modelPtr, int indexBefore)
	{
		if (auto listOwner = getParentModel()) {
			if (auto dataModel = listOwner->getDataModel())
			{
				auto standaloneModelPtr = dataModel->takeStandaloneModel(modelPtr.get());
				if (!standaloneModelPtr) // Already taken
					standaloneModelPtr = modelPtr;

				const VLVarModelPtr* ret = &nullVarModelPtr;
				auto insertRet = addData(vl::MakePtr(standaloneModelPtr->getData()), indexBefore,
					[&] {
						beginInsertNewRow();
					}, [&] (int index) {
						ret = &putModel(index, standaloneModelPtr, indexBefore);
						endInsertRows();
					}
				);

				LOCAL_DEBUG("Put '[" << insertRet.index << "] "
					 << (standaloneModelPtr->isObject() ?
							 standaloneModelPtr->asObject()->getData().Get("name").AsString().Val().c_str()
						   : "")
					 << "' (" << standaloneModelPtr.get() << ")");

				return *ret;
			}
		}
		return nullVarModelPtr;
	}

	ModelListInsertRet VLListModel::addDataAndLoadModel(const vl::VarPtr &value, int indexBefore)
	{
		const VLVarModelPtr* ret = nullptr;
		auto insertRet = addData(value, indexBefore,
			[&] {
				beginInsertNewRow();
			}, [&] (int index) {
				ret = &modelAt(index);
				endInsertRows();
			}
		);
		return {insertRet.index, *ret};
	}

	vl::ListInsertRet VLListModel::addData(
			const vl::VarPtr &ptr
			, int indexBefore
			, const VoidCb& onBeforeAdd
			, const VoidCbInt& onAfterAdd
			)
	{
		vl::ListInsertRet result;
		if (onBeforeAdd)
			onBeforeAdd();
		result = getData().Add(ptr, indexBefore);
		if (onAfterAdd)
			onAfterAdd(result.index);
		if (indexBefore >= 0)
		{
			auto sz = size();
			for (int i = indexBefore; i < sz; i++)
				onModelChanged(i);
		}
		return result;
	}

	vl::ListInsertRet VLListModel::setData(int index, const vl::VarPtr &ptr)
	{
		return getData().Set(index, ptr);
		// TODO: emit update signals
	}

	vl::ListInsertRet VLListModel::addData(const vl::Var &v, int indexBefore)
	{
		return addData(vl::MakePtr(v), indexBefore);
	}
	// End of data and model setters

	QVariant VLListModel::find(const QVariant &data)
	{
		VLVarModel* result = nullptr;
		foreachElement([&](int index, const VLVarModelPtr& model) {
			if (data.canConvert<VLVarModel*>())
			{
				if (model.get() == qvariant_cast<VLVarModel*>(data))
					result = model.get();
			}
			else
			{
				const VLVarModel* m = model.get();
				switch (data.userType())
				{
				case QMetaType::QString:
					if (m->getData().AsString().Val() == data.toString().toStdString())
						result = model.get();
					break;
				case QMetaType::Int:
				case QMetaType::Double:
				case QMetaType::Float:
					if (m->getData().AsNumber().Val() == data.toInt())
						result = model.get();
					break;
				case QMetaType::Bool:
					if (m->getData().AsBool().Val() == data.toBool())
						result = model.get();
				}
			}
			if (result != nullptr)
				return false;
			return true;
		});
		return QVariant::fromValue(result);
	}

	const VLVarModelPtr &VLListModel::putModel(int index, const VLVarModelPtr &ptr, int indexBefore) {
		if (auto& ret = getListStorage().put(index, ptr, indexBefore))
		{
			if (auto listOwner = getParentModel())
			{
				if (!ret->getParentModel())
					ret->Init(listOwner);
			}
			else
				LOCAL_ERROR("putModel(" << index << ") no parent present in the list model. The putting model won't be initialized");
			return ret;
		}
		else
		{
			LOCAL_ERROR("putModel(" << index << ") return nullptr through the storage");
		}
		return nullVarModelPtr;
	}

	bool VLListModel::loadElementModels()
	{
		auto sz = dataSize();
		getListStorage().resize(sz);
		for (int i = 0; i < sz; i++)
			if (!loadElementModel(i))
				return false;
		return true;
	}

	// Common virtual method
	// Do the same as putModel
	// Redirect to putModel
	const VLVarModelPtr &VLListModel::setModelAt(int index, const VLVarModelPtr &m)
	{
		if (!m)
		{
			LOG_ERROR("Null model passed to setModelAt('" << index << ")");
			return nullVarModelPtr;
		}

		vl::VarPtr dataPtr = nullptr;
		VLVarModelPtr modelPtr = nullptr;

		// Check if it is a standalone model
		if (!m->getParentModel())
			if (auto owner = m->getDataModel())
				if (auto standaloneModel = owner->takeStandaloneModel(m.get()))
				{
					modelPtr = standaloneModel;
					dataPtr = vl::MakePtr(m->getData());
				}
		// Otherwise just take its data as a copy
		// And return a newly created model
		if (!modelPtr)
			dataPtr = m->getData().Copy();

		// Set the data
		setData(index, dataPtr);
		if (modelPtr)
			return putModel(index, modelPtr);
		else
			return modelAt(index);
	}


	const VLVarModelPtr& VLListModel::loadElementModel(int index, int indexBefore)
	{
		if (index >= dataSize())
			return nullVarModelPtr;
		if (auto parent = getParentModel())
		{
			// Container should be prepared to fit in the range
			auto& ptr = putModel(index
						 , VarModelFactory::Instance().CreateEmpty(getData(index))
						 , indexBefore);
#ifdef LOG_ON
		qDebug() << "Put '[" << index << "] "
				 << (ptr->isObject() ?
						 ptr->asObject()->getData().Get("name").AsString().Val().c_str()
					   : "")
				 << "' (" << ptr.get() << ")";
#endif
			return ptr;
		}
		else
		{
			// Parent should exist
			assert(false);
		}
		return nullVarModelPtr;
	}

	const vl::Var &VLListModel::setElementValue(const VLVarModel *e, const vl::VarPtr &value)
	{
		auto index = getElementIndex(e);
		if (index >= 0)
			return setData(index, value).data();
		return vl::emptyVar;
	}

	bool VLListModel::setElementType(const VLVarModel *e, ObjectProperty::Type newType)
	{
		if (auto index = getElementIndex(e))
			return setType(index, newType);
		return false;
	}

	bool VLListModel::setType(int index, ObjectProperty::Type type)
	{
		if (auto& model = getStorage().at(index))
			if (ObjectProperty::ConvertVLType(model->getData()) != type)
			{
				auto newDataPtr = ObjectProperty::MakeVarPtr(type);
				auto newModel = VarModelFactory::Instance().Create(*newDataPtr);
				return setModelAt(index, newModel) != nullptr;
			}
		return false;
	}

	bool VLListModel::doSetData(int index, const QVariant &value, int role)
	{
		switch (role) {
		case RoleType:
				// Emit dataChanged signal
				return setType(index, qvariant_cast<ObjectProperty::Type>(value));
			case RoleValue:
				// Emit dataChanged signal
				return setData(index, ObjectProperty::makeVarFromData(value));
		}
		return false;
	}
}
