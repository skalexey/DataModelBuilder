#ifndef VLLISTMODELINTERFACE_H
#define VLLISTMODELINTERFACE_H

#include <utility>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <QAbstractListModel>
#include <QVariantList>
#include <utils/common.h>
#include "vl_fwd.h"
#include "ObjectProperty.h"
#include "ModelStorage.h"
#include "ModelsFwd.h"

namespace dmb
{
	class VLListModelInterface : public QAbstractListModel
	{
		typedef QAbstractListModel Base;

		friend class ListModelStorageSubscriptionProcessor;
		friend class ModelStorageSubscriptionProcessor;

		Q_OBJECT
	protected:
		enum Role {
			RoleId,
			RoleType,
			RoleTypeStr,
			RoleValue,
			RoleValueStr,
			RoleParent,
			InterfaceRoleCount
		};

		// Main access interface:
		// getAt
		// at
		// atSp
		// getAtSp
	public:
		// Constructors and initializers
		explicit VLListModelInterface(QObject* parent = nullptr);
		explicit VLListModelInterface(QObject* parentModel, const ModelStoragePtr& storage);
		explicit VLListModelInterface(QObject* parentModel, const VLListModelInterface& storageOwner);
		virtual ~VLListModelInterface();
		virtual bool Init(QObject* parentModel);
		virtual bool Init(const VLVarModelPtr& parentModel);
		// Strictly necessary
		virtual int dataSize() const = 0;
		// Needed in all collections because of list-like representation
		// of the ListModel in Qt
		virtual bool doRemove(int index) = 0; // return true if success
		// Non-const version of getAtSp
		// Retrieve a raw pointer from the storage to a model at index
		// Create a model if it doesn't exists
		virtual const dmb::VLVarModelPtr& modelAt(int index) = 0;
		virtual const VLVarModelPtr& getModelAt(int index) const = 0;
		virtual bool loadElementModels() = 0;
		virtual const VLVarModelPtr& setModelAt(int index, const VLVarModelPtr& modelPtr) = 0;
		virtual bool doSetData(int index, const QVariant& value, int role) = 0;
		virtual const vl::Var& setElementValue(const VLVarModel* e, const vl::VarPtr& value) = 0;
		virtual bool setElementType(const VLVarModel *childPtr, ObjectProperty::Type type) = 0;

	protected:
		virtual const ModelStorage& getStorage() const {
			// Should always be
			return *mStorage;
		}
		const ModelStoragePtr& getSharedStorage() const {
			return mStorage;
		}

		// Begin of Common code which should be run on any storage holder
		// when a model is put into it
		void onModelPut(const VLVarModelPtr& model);
		void onModelBeforeRemove(const VLVarModelPtr& m);
		// End of Common code which should be run on any storage holder

	public:
		// Other
		virtual int size() const;
		virtual void clear(); // Clear the list
		// Clears the model storage and call begin/endRemoveRows
		// Not needed in subscribed approach
		void clearAndNotify();
		bool elementsLoaded() const;
		void connectSignals(VLVarModel* model) const;
		void disconnectSignals(VLVarModel* model) const;
		virtual VLCollectionModel* asCollection();
		virtual VLListModel* asList();
		virtual bool isCollection() const;
		virtual bool isList() const;
		// Role getters
		virtual QVariant role(const VLVarModelPtr& m, int index, int role) const;
		virtual QVariant roleType(const VLVarModelPtr& m, int index) const;
		virtual QVariant roleTypeStr(const VLVarModelPtr& m, int index) const;
		virtual QVariant roleValueStr(const VLVarModelPtr& m, int index) const;
		virtual QVariant roleValue(const VLVarModelPtr& m, int index) const;
		// Write methods

	public:
		// QAbstractListModel interface
		int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
		// Editable:
		bool setData(const QModelIndex &index, const QVariant &value,
					 int role = Qt::EditRole) override;
		Qt::ItemFlags flags(const QModelIndex& index) const override;
		QHash<int,QByteArray> roleNames() const override;

	public:
		// Public data interface

	protected:
		// Protected data interface
		// Non-const mirror implementation of getData(int index) const
		const vl::Var& getData(int index) const;

	public:
		// Public Qt model interface
		const VLVarModelPtr getParentModel() const {
			return mParentModel.lock();
		}
		virtual int getElementIndex(const void* e) const {
			return getStorage().getIndex(e);
		}
		bool foreachElement(const std::function<bool(int, const VLVarModelPtr&)>& pred, bool recursive = false) const;
		bool foreachElement(const std::function<bool(int, const VLVarModelPtr&)>& pred, bool recursive = false);

	protected:
		// Protected Qt model interface
		VLVarModelPtr getParentModel() {
			return mParentModel.lock();
		}

	protected:
		// Other
		void resetList(const utils::void_cb& doWithList = nullptr);
		ModelStorage& getStorage() {
			return const_cast<ModelStorage&>(
						const_cast<const VLListModelInterface*>(this)->getStorage()
					);
		}
		void beginInsertNewRow();

	public:
		// Properties
		Q_INVOKABLE QVariant at(int index);
		Q_INVOKABLE virtual bool removeAt(int index);
		Q_PROPERTY (int size READ size NOTIFY sizeChanged);
		Q_PROPERTY (dmb::VLVarModel* parent READ getParentModelProp NOTIFY parentModelChanged);

	protected:
		// Properties implementation
		dmb::VLVarModel *getParentModelProp() const;
		virtual std::unique_ptr<ModelStorageSubscriptionProcessor> createStorageSubscriptionProcessor();

	public slots:
		virtual void onNameChanged();
		void onValueChanged();
		void onTypeChanged();
		void onModelChanged(int indexFirst, int indexLast = -1);

	signals:
		void sizeChanged(int newSize);
		void parentModelChanged();

	protected:
		std::unique_ptr<ModelStorageSubscriptionProcessor> mStorageSubscriptionProcessor;

	private:
		// Data
		std::weak_ptr<VLVarModel> mParentModel;
		ModelStoragePtr mStorage = nullptr;
	};
}

#endif // VLLISTMODELINTERFACE_H
