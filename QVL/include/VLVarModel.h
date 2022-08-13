#ifndef VLVARMODEL_H
#define VLVARMODEL_H

#include <memory>
#include <string>
#include <QObject>
#include "vl_fwd.h"
#include "ObjectProperty.h"
#include "ModelsFwd.h"

namespace dmb
{
	class VLVarModel : public QObject, public std::enable_shared_from_this<VLVarModel>
	{
		friend class VLListModelInterface;
		friend class VLCollectionModel;
		friend class VLListModel;
		friend class ModelStorage;
		friend class ObjectModelStorage;
		friend class ListModelStorage;

		Q_OBJECT

	public:
		// Constructors and initializers
		explicit VLVarModel(QObject *parent = nullptr);
		VLVarModel(const vl::Var& v, QObject *parent = nullptr);
		virtual ~VLVarModel();
		virtual void Init(QObject* parent = nullptr);
		virtual void Init(QObject *parent, DMBModel* owner);
		virtual void Init(QObject *parent, const vl::Var& data, DMBModel* owner);
		virtual void Init(const VLVarModelPtr& parent = nullptr);
		// Type casts
		virtual const VLObjectVarModel* asObject() const;
		virtual VLObjectVarModel* asObject();
		virtual const VLListVarModel* asList() const;
		virtual VLListVarModel* asList();
		// Type checks
		virtual bool isObject() const;
		virtual bool isList() const;

	public:
		// Qt Interface
		Q_PROPERTY (QString id READ id WRITE setId NOTIFY idChanged)
		Q_PROPERTY (QString name READ id WRITE setId NOTIFY idChanged) // Alias for id property
		Q_PROPERTY (ObjectProperty::Type type READ type WRITE setType NOTIFY typeChanged)
		Q_PROPERTY (QVariant value READ value WRITE setValue NOTIFY valueChanged)
		Q_PROPERTY (QVariant valueStr READ valueStr NOTIFY valueChanged)
		Q_PROPERTY (QVariant typeStr READ typeStr NOTIFY typeChanged)
		Q_PROPERTY (QObject* owner READ getDataModel NOTIFY ownerChanged)
		Q_PROPERTY (QVariant parent READ parentModelProp WRITE setParentModel NOTIFY parentChanged)
		Q_INVOKABLE bool remove();
		Q_INVOKABLE bool detachFromParent();
		Q_INVOKABLE QVariant copy();

	public:
		VLVarModelPtr getParentModel() const;
		DMBModel* getDataModel();
		const DMBModel* getDataModel() const;
		const vl::Var& getData() const;
		std::string getStringId() const;

	public:
	signals:
		void idChanged() const;
		void typeChanged() const;
		void valueChanged() const;
		void ownerChanged(dmb::DMBModel* newOwner) const;
		void beforeRemove() const;
		void removed() const;
		void parentChanged() const;

	protected:
		// For Qt Interface
		QVariant parentModelProp();
		VLVarModelPtr parentModel();
		bool setParentModel(const QVariant& data);
		virtual bool removeChild(const VLVarModel* childPtr);
		virtual const VLVarModelPtr& getChild(const VLVarModel* p) const;
		virtual const VLVarModelPtr& getPtr();
		QString id() const;
		bool setId(const QString &newId);
		ObjectProperty::Type type() const;
		QString typeStr() const;
		bool setType(const ObjectProperty::Type &newType);
		// Destroys the child model and replace it by a new with newType
		virtual bool setChildType(const VLVarModel* child, const ObjectProperty::Type &newType) {
			// Default logic
			return false;
		}
		virtual const vl::Var& setChildValue(const VLVarModel* child, const vl::VarPtr& value);
		QVariant value() const;
		bool setValue(const QVariant &newValue);
		QVariant valueStr() const;

	protected:
		// Protected data interface
		vl::Var& data();
		virtual vl::Var& getChildData(const VLVarModel* childPtr);
		void setDataModel(DMBModel* m);
		const std::string& getId() const;
		int getIndex() const;
		virtual const vl::Var& getChildData(const VLVarModel* childPtr) const;
		std::string getTypeId() const;
		bool setId(const std::string &newId);

	private:
		// Data
		vl::VarPtr mVarPtr = nullptr;
		std::weak_ptr<VLVarModel> mParentModel;
		DMBModel* mDMBModel = nullptr;
	};

	typedef std::shared_ptr<VLVarModel> VLVarModelPtr;
	extern VLVarModelPtr nullVarModelPtr;
}

#endif // VLVARMODEL_H
