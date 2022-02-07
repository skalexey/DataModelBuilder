#ifndef VLLISTMODELINTERFACE_H
#define VLLISTMODELINTERFACE_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <QAbstractListModel>
#include <QVariantList>
#include "vl_fwd.h"
#include "ObjectProperty.h"
#include "ModelsFwd.h"

namespace dmb
{
	class VLListModelInterface : public QAbstractListModel
	{
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

	public:
		// Constructors and initializers
		explicit VLListModelInterface(QObject* parentModel);
		~VLListModelInterface();
		virtual void Init(QObject* parentModel);
		virtual void Init(const VLVarModelPtr& parentModel);
		// Strictly necessary
		virtual int dataSize() const = 0;
		virtual const vl::Var& getDataAt(int index) const = 0;
		virtual bool doRemove(int index) = 0; // return true if success

	protected:
		virtual vl::Var& setDataAt(int index, const vl::VarPtr& ptr) = 0;

	public:
		// Other
		virtual int size() const;
		virtual void clear(); // Clear the list
		bool elementsLoaded() const;
		bool loadElementModels();
		// Role getters
		virtual QVariant role(const VLVarModel* m, int index, int role) const;
		virtual QVariant roleType(const VLVarModel* m, int index) const;
		virtual QVariant roleTypeStr(const VLVarModel* m, int index) const;
		virtual QVariant roleValueStr(const VLVarModel* m, int index) const;
		virtual QVariant roleValue(const VLVarModel* m, int index) const;
		// Write methods
		virtual bool doSetData(int index, const QVariant& value, int role);

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
		bool setType(int index, ObjectProperty::Type type);

	protected:
		// Protected data interface
		// Non-const mirror implementation of getDataAt(int index) const
		virtual vl::Var& getDataAt(int index);
		// Setters
		vl::Var& setDataAt(int index);
		vl::Var& setDataAt(int index, const vl::Var& value);
		template <typename T>
		vl::Var& setDataAt(int index, const T& value)
		{
			return setDataAt(index, MakePtr(value));
		}

	public:
		// Public Qt model interface
		const dmb::VLVarModelPtr& getAtSp(int index) const;
		const dmb::VLVarModel* getAt(int index) const;
		const VLVarModelPtr getParentModel() const;
		int getElementIndex(const VLVarModel* elementPtr) const;
		bool foreachElement(const std::function<bool(int, const VLVarModelPtr&)>& pred) const;
		bool foreachElement(const std::function<bool(int, const VLVarModelPtr&)>& pred);

	protected:
		// Protected Qt model interface
		VLVarModelPtr loadElementModel(int index, bool resize = true);
		VLVarModelPtr getParentModel();

	protected:
		// Other
		void resetList(const std::function<void()>& doWithList = nullptr);

	public:
		// Properties
		Q_INVOKABLE dmb::VLVarModel* at(int index);
		Q_INVOKABLE bool removeAt(int index);
		Q_PROPERTY (int size READ size NOTIFY sizeChanged);

	public slots:
		void onValueChanged(int index);
		void onTypeChanged(int index);

	signals:
		void sizeChanged(int newSize);

	protected:
		void putModel(int index, const VLVarModelPtr& ptr);

	private:
		// Data
		class MStorage
		{
		public:
			int getIndex(const VLVarModel* e) const;
			int size() const;
			void resize(int newSize);
			void clear();
			void put(int index, const VLVarModelPtr& ptr);
			const VLVarModelPtr& operator[](int index) const;
			VLVarModelPtr& at(int index);
			void remove(int index);

		protected:
			std::vector<VLVarModelPtr> mElements;
			std::map<const VLVarModel*, int> mElementIndex;
		};
		MStorage mStorage;
		std::weak_ptr<VLVarModel> mParentModel;
	};
}

#endif // VLLISTMODELINTERFACE_H
