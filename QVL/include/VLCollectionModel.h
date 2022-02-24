#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include "VLListModelInterface.h"
#include "VLVarModel.h"

namespace dmb
{
	class VLObjectVarModel;

	class VLCollectionModel: public VLListModelInterface
	{
		typedef VLListModelInterface Base;

		Q_OBJECT

	protected:
		enum CollectionRole {
			RoleName = InterfaceRoleCount,
			RoleParent
		};

	public:
		// Constructors and initializers
		explicit VLCollectionModel(QObject *parent = nullptr);
		bool Init(const VLVarModelPtr& objectPtr) override;
		void UpdateIdList();

	public:
		// ======= Begin of VLListModelInterface interface =======
		// Strictly necessary
		virtual int dataSize() const override;
		const vl::Var& getDataAt(int index) const override;
		bool doRemove(int index) override;

	protected:
		vl::Var& setDataAt(int index, const vl::VarPtr& ptr, const std::function<VLVarModelPtr(bool alreadyExist)>& customModelLoader = nullptr) override;

	public:
		// Other
		bool doSetData(int index, const QVariant& value, int role) override;
		void clear() override;
		QVariant role(const VLVarModel* m, int index, int role) const override;
		QVariant roleValueStr(const VLVarModel* m, int index) const override;
		QHash<int,QByteArray> roleNames() const override;
		// ======= End of VLListModelInterface interface =======

	public:
		// Public data interface
		const vl::Object& getData() const;
		const vl::Var& getData(const std::string& propId) const;
		vl::Var& getData(const std::string& propId);
		const std::string& getId(int index) const;
		int getIndex(const std::string& propId) const;
		bool hasData(const std::string& propId) const;
		vl::Var& setData(const std::string& propId);
		vl::Var& setData(const std::string& propId, const vl::Var& value);
		vl::Var& setData(const std::string& propId, const vl::VarPtr& value, const std::function<VLVarModelPtr(bool alreadyExist)>& customModelLoader = nullptr);
		template <typename T>
		vl::Var& setData(const std::string& propId, const T& value)
		{
			return setData(propId, MakePtr(value));
		}
		bool addData(const std::string& propId, ObjectProperty::Type type);

	protected:
		// Protected data interface
		vl::Object& getData();
		std::string& getId(int index);

	public:
		// Public Qt model interface
		bool has(const std::string& propId) const;
		VLObjectVarModelPtr getParentModel() const;
		const VLVarModelPtr& getModelSp(const std::string& propId) const;
		// Non-const version of getModelSp
		const VLVarModelPtr& modelSp(const std::string& propId);
		const VLVarModelPtr& setModel(const std::string& propId, const VLVarModelPtr& modelPtr);
		const VLVarModel* getModel(const std::string& propId) const;
		// Non-const version of getModel()
		VLVarModel* model(const std::string& propId);
		bool renameElement(const std::string& propId, const std::string& newId);
		bool renameElement(int index, const std::string& newId);
		bool remove(const std::string& propId);

	protected:
		// Protected Qt model interface
		VLObjectVarModelPtr getParentModel();

	public:
		// Properties
		Q_INVOKABLE bool has(const QString& propId) const;
		Q_INVOKABLE dmb::VLVarModel* get(const QString& propId);
		Q_INVOKABLE dmb::VLVarModel* set(const QString& propId, dmb::VLVarModel* v);
		Q_INVOKABLE dmb::VLVarModel* set(const QString& propId, const QVariant& data);
		// New item will be created. If the passed name exists then a number postfix will be added
		Q_INVOKABLE bool add(const QString& propId, ObjectProperty::Type type);
		Q_INVOKABLE bool add(ObjectProperty::Type type);
		Q_INVOKABLE bool remove(const QString& propId);

	public slots:
		void onNameChanged(int index) override;

	signals:
		void nameAlreadyTaken();

	private:
		// Data
		std::unordered_map<std::string, int> mPropIndex;
		std::vector<std::string> mIdList;
	};
}
#endif // PARAMETERLISTMODEL_H
