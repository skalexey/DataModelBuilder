#ifndef VLLISTMODEL_H
#define VLLISTMODEL_H

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include "VLListModelInterface.h"
#include "vl.h"
#include "ModelsFwd.h"
#include "ListModelStorage.h"
#include "ListModelSubscriptionProcessor.h"

namespace dmb
{
	struct ModelListInsertRet {
		int index;
		const VLVarModelPtr& data;
		operator bool() { return data != nullptr; }
	};

	class VLListModel: public VLListModelInterface
	{
		typedef VLListModelInterface Base;

		friend class VLListVarModel;
		friend class ListModelSubscriptionProcessor;

		Q_OBJECT

	public:
		// Constructors and initializers
		explicit VLListModel(QObject* parent = nullptr);
		explicit VLListModel(QObject *parent, const ListModelStoragePtr& storage);
		explicit VLListModel(QObject* parent, const VLListModelInterface& storageOwner);
		~VLListModel();
		bool Init(const VLVarModelPtr& newParent) override;

	public:
		// ======= Begin of VLListModelInterface interface =======
		// Strictly necessary
		virtual int dataSize() const override;
		bool doRemove(int index) override;
		const dmb::VLVarModelPtr& modelAt(int index) override;
		const VLVarModelPtr& getModelAt(int index) const override;
		bool loadElementModels() override;
		const VLVarModelPtr& setModelAt(int index, const VLVarModelPtr& modelPtr) override;
		const VLVarModelPtr& loadElementModel(int index, int indexBefore = -1);
		const vl::Var& setElementValue(const VLVarModel *e, const vl::VarPtr &value) override;
		bool setElementType(const VLVarModel *e, ObjectProperty::Type newType) override;
		bool doSetData(int index, const QVariant& value, int role) override;

	public:
		// Other
		QHash<int,QByteArray> roleNames() const override;
		bool isList() const override;
		VLListModel* asList() override;
		// ======= End of VLListModelInterface interface =======

	public:
		// Public data interface
		const vl::List& getData() const;
		const vl::Var& getData(int index) const;
		bool setType(int index, ObjectProperty::Type type);

	protected:
		// Protected data interface
		vl::List& getData();
		vl::Var& getData(int index);
		// Data setters used only under the hood
		vl::ListInsertRet addData(const vl::Var& v, int indexBefore = -1);
		vl::ListInsertRet addData(const vl::VarPtr& ptr, int indexBefore = -1
				, const VoidCb& onBeforeAdd = nullptr
				, const VoidCbInt& onAfterAdd = nullptr
		);
		vl::ListInsertRet setData(int index, const vl::VarPtr& ptr);
		// Helper data setters
		ModelListInsertRet addDataAndLoadModel(
				const vl::VarPtr& value
				, int indexBefore = -1
		);
	public:
		// Public Qt model interface
		VLListVarModelPtr getParentModel() const;
		QVariant add(dmb::VLVarModel* model, int indexBefore = -1);
		const VLVarModelPtr& addModel(const VLVarModelPtr& modelPtr, int indexBefore = -1);

	public:
		// Properties
		Q_INVOKABLE QVariant add(ObjectProperty::Type type, int indexBefore = -1);
		Q_INVOKABLE QVariant add(const QVariant& data, int indexBefore = -1);
		Q_INVOKABLE QVariant find(const QVariant& data);

	protected:
		// Storage interface
		inline ListModelStorage& getListStorage() {
			return *std::dynamic_pointer_cast<ListModelStorage>(getSharedStorage());
		}

		inline const ListModelStorage& getListStorage() const{
			return *std::dynamic_pointer_cast<ListModelStorage>(getSharedStorage());
		}

		const VLVarModelPtr& putModel(int index, const VLVarModelPtr& ptr, int indexBefore = -1);

	protected:
		// Other
		virtual std::unique_ptr<ListModelSubscriptionProcessor> createVLSubacriptionProcessor();

	protected:
		// Composition
		std::unique_ptr<ListModelSubscriptionProcessor> mVLSubscriptionProcessor;
	};
}
#endif // VLLISTMODEL_H
