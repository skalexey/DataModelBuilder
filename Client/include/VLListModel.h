#ifndef VLLISTMODEL_H
#define VLLISTMODEL_H

#include <string>
#include <vector>
#include <unordered_map>
#include "VLListModelInterface.h"
#include "vl.h"

namespace dmb
{
	class VLListVarModel;

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
		vl::Var& setDataAt(int index, const vl::VarPtr& ptr) override;

	public:
		// Other
		QHash<int,QByteArray> roleNames() const override;
		// ======= End of VLListModelInterface interface =======

	public:
		// Public data interface
		const vl::List& getData() const;
		vl::Var& addData(ObjectProperty::Type type);

	protected:
		// Protected data interface
		vl::List& getData();

	public:
		// Public Qt model interface
		const VLListVarModel* getParentModel() const;

	public:
		// Properties
		Q_INVOKABLE bool add(ObjectProperty::Type type);

	protected:
		// Protected Qt model interface
		VLListVarModel* getParentModel();
	};
}
#endif // VLLISTMODEL_H
