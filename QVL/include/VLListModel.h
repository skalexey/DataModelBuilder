#ifndef VLLISTMODEL_H
#define VLLISTMODEL_H

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include "VLListModelInterface.h"
#include "vl.h"
// Needed for Q_INVOKABLE VLVarModel*
#include "VLVarModel.h"

namespace dmb
{
	class VLListModel: public VLListModelInterface
	{
		typedef VLListModelInterface Base;

		Q_OBJECT

		enum ListRole
		{
			RoleParentModel = InterfaceRoleCount
		};

	public:
		// Constructors and initializers
		explicit VLListModel(QObject *parent = nullptr);

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
		QHash<int,QByteArray> roleNames() const override;
		// ======= End of VLListModelInterface interface =======

	public:
		// Public data interface
		const vl::List& getData() const;
		vl::Var& addData(ObjectProperty::Type type, int indexBefore = -1);
		vl::Var& addData(const vl::Var& v, int indexBefore = -1);
		vl::Var& addData(const vl::VarPtr& ptr, int indexBefore = -1, const std::function<void(int newIndex)>& customModelLoader = nullptr);

	protected:
		// Protected data interface
		vl::List& getData();

	public:
		// Public Qt model interface
		VLListVarModelPtr getParentModel() const;
		VLVarModel* add(const dmb::VLVarModel* model, int indexBefore = -1);
		const VLVarModelPtr& addModel(const VLVarModelPtr& modelPtr, int indexBefore = -1);

	public:
		// Properties
		Q_INVOKABLE dmb::VLVarModel* add(ObjectProperty::Type type, int indexBefore = -1);
		Q_INVOKABLE dmb::VLVarModel* add(const QVariant& data, int indexBefore = -1);
		Q_INVOKABLE dmb::VLVarModel* find(const QVariant& data);
	};
}
#endif // VLLISTMODEL_H
