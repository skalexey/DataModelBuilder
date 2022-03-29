#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include <utility>
#include <memory>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include "VLListModelInterface.h"
#include "ModelsFwd.h"
#include "ObjectModelStorage.h"
#include "vl_fwd.h"
#include "CollectionModelSubscriptionProcessor.h"

namespace dmb
{
	class VLObjectVarModel;

	struct ModelInsertRet {
		std::string id;
		const VLVarModelPtr& data;
		operator bool() { return data != nullptr; }
	};

	class VLCollectionModel: public VLListModelInterface
	{
		typedef VLListModelInterface Base;

		friend class CollectionModelSubscriptionProcessor;
		friend class ObjectModelStorageSubscriptionProcessor;

		Q_OBJECT

	protected:
		enum CollectionRole {
			RoleName = InterfaceRoleCount
		};

	public:
		// Constructors and initializers
		explicit VLCollectionModel(QObject* parent = nullptr);
		explicit VLCollectionModel(QObject *parent, const ObjectModelStoragePtr& storage);
		explicit VLCollectionModel(QObject* parent, const VLListModelInterface& storageOwner);
		~VLCollectionModel();
		bool Init(const VLVarModelPtr& newParent) override;

	public:
		// ======= Begin of VLListModelInterface interface =======
		// Strictly necessary
		bool doRemove(int index) override;
		const VLVarModelPtr& modelAt(int index) override;
		const VLVarModelPtr& getModelAt(int index) const override;
		const VLVarModelPtr& setModelAt(int index, const VLVarModelPtr& modelPtr) override;
		bool loadElementModels() override;
		bool doSetData(int index, const QVariant& value, int role) override;
		const vl::Var &setElementValue(const VLVarModel *e, const vl::VarPtr &value) override;
		bool setElementType(const VLVarModel *e, ObjectProperty::Type newType) override;

	public:
		// Other
		QVariant role(const VLVarModelPtr& m, int index, int role) const override;
		QVariant roleValueStr(const VLVarModelPtr& m, int index) const override;
		int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		QHash<int,QByteArray> roleNames() const override;
		bool isCollection() const override;
		VLCollectionModel* asCollection() override;
		// ======= End of VLListModelInterface interface =======

		virtual std::string getFreeId(const std::string& desiredId) const;
		inline const std::string* getElementId(const void* e) const {
			return getObjectStorage().getId(e);
		}

	public:
		// Public data interface
		// Returns id by the index belonging the local id list indexation space
		virtual inline const std::string* getId(int index) const {
			return getObjectStorage().getId(index);
		}
		// Returns index in the local id list indexation space of the given id
		virtual inline int getIndex(const std::string& propId) const {
			return getObjectStorage().getIdList().index(propId);
		}

	public:
		// Public model interface
		VLObjectVarModelPtr getParentModel() const;
		virtual const VLVarModelPtr& getModel(const std::string& propId) const;
		// Non-const version of getModel
		virtual const VLVarModelPtr& model(const std::string& propId);

		virtual const VLVarModelPtr& setModel(const std::string& propId, const VLVarModelPtr& modelPtr);
		// Used for JS public set(QVariant) if the model is passed
		virtual bool renameElement(const std::string& propId, const std::string& newId);

		// Remove a model with it's data from the object and this collection
		virtual bool remove(const std::string& propId);
		// Redirects to remove
		bool removeElement(const VLVarModel *e);

		virtual void UpdateIdList();

	public:
		// Qt interface
		Q_INVOKABLE bool has(const QString& propId) const;
		Q_INVOKABLE QVariant get(const QString& propId);
		Q_INVOKABLE QVariant set(const QString& propId, const QVariant& data);
		// Adds a new model with propId and type to the end of the list
		// stores and return it
		// If the passed id is already taken
		// then take the free similar to the passed with an added suffix
		Q_INVOKABLE QVariant add(const QString& propId, ObjectProperty::Type type);
		// Redirects to add(const QString&)
		Q_INVOKABLE QVariant add(ObjectProperty::Type type);
		Q_INVOKABLE bool remove(const QString& propId);
		Q_INVOKABLE bool removeAt(int index) override;

	public slots:
		void onNameChanged() override;
		void onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
		void onRowsInserted(const QModelIndex &parent, int first, int last);
		void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
		void onRowsRemoved(const QModelIndex &parent, int first, int last);


	signals:
		void nameAlreadyTaken();

	protected:
		// Protected data interface
		virtual int dataSize() const override;
		vl::Object& getData();
		const vl::Object& getData() const;
		const vl::Var& getData(const std::string& propId) const;
		vl::Var& getData(const std::string& propId);
		bool hasDataOwn(const std::string& propId) const;
		// Data setters used only under the hood
		vl::Var& setData(const std::string& propId);
		vl::Var& setData(const std::string& propId, const vl::Var& value);
		// Set the value to propId and call onBeforeAdd before the data setting
		// and onAfterAdd after the data is set
		// if a new property is added
		virtual vl::Var& setData(
				const std::string& propId
				, const vl::VarPtr& value
				, const VoidCb& onBeforeAdd = nullptr
				, const VoidCb& onAfterAdd = nullptr
				);
		template <typename T>
		vl::Var& setData(const std::string& propId, const T& value)
		{
			return setData(propId, MakePtr(value));
		}
		bool setType(const std::string& propId, ObjectProperty::Type type);
		// Helper data-model setters
		virtual const VLVarModelPtr& setDataAndLoadModel(
				const std::string& propId
				, const vl::VarPtr& value
		);

		ModelInsertRet addDataAndLoadModel(
				const std::string& propId
				, const vl::VarPtr& value
		);

		// Used as a helper for Qt interface function set()
		const VLVarModelPtr& set(const std::string& propId, dmb::VLVarModel* v);

	protected:
		// Functions to support custom id list
		// to be able work in their own indexation space

		// indexBefore is not used on the higher levels now
		inline virtual const VLVarModelPtr& putToStorage(
				const std::string& propId
				, const VLVarModelPtr& model
				, int indexBefore = -1) {
			return getObjectStorage().put(propId, model, indexBefore);
		}

	protected:
		// Protected Qt model interface
		VLObjectVarModelPtr getParentModel();
		virtual std::unique_ptr<CollectionModelSubscriptionProcessor> createVLSubacriptionProcessor();

	protected:
		// Storage interface
		inline ObjectModelStorage& getObjectStorage() {
			return *std::dynamic_pointer_cast<ObjectModelStorage>(getSharedStorage());
		}

		inline const ObjectModelStorage& getObjectStorage() const{
			return *std::dynamic_pointer_cast<ObjectModelStorage>(getSharedStorage());
		}

		// Store the model ptr in the storage
		// and init it with the parent (this collection's owner)
		// indexBefore is not used on the higher levels now
		const VLVarModelPtr& putModel(
				const std::string& propId
				, const VLVarModelPtr& ptr
				, int indexBefore = -1
		);

	protected:
		// Other
		// Create a model and store it
		// indexBefore is not used on the higher levels now
		virtual const VLVarModelPtr& loadElementModel(const std::string& propId, int indexBefore = -1);
		// Called from loadElementModel for it's code reuse
		virtual const VLVarModelPtr& loadProtoModel(const vl::Var& data, const VLObjectVarModelPtr& collectionOwner, int indexBefore = -1);

		// Custom storage subscription processor creator
		std::unique_ptr<ModelStorageSubscriptionProcessor> createStorageSubscriptionProcessor() override;

	protected:
		// Composition
		std::unique_ptr<CollectionModelSubscriptionProcessor> mVLSubscriptionProcessor;
	};
}
#endif // PARAMETERLISTMODEL_H
