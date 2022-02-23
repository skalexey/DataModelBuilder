#ifndef VLVARMODEL_H
#define VLVARMODEL_H

#include <memory>
#include <string>
#include <QObject>
#include "vl_fwd.h"
#include "ObjectProperty.h"
#include "ModelsFwd.h"

QT_BEGIN_NAMESPACE
		class QModelIndex;
QT_END_NAMESPACE

namespace dmb
{
	class VLVarModel : public QObject
	{
		friend class VLListModelInterface;

		Q_OBJECT

	public:
		// Constructors and initializers
		explicit VLVarModel(QObject *parent = nullptr);
		VLVarModel(const vl::Var& v, QObject *parent = nullptr);
		~VLVarModel();
		virtual void Init(QObject* parent = nullptr);
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
		// Public data interface
		const std::string& getId() const;
		const vl::Var& getData() const;
		virtual const vl::Var& getChildData(const VLVarModel* childPtr) const;
		std::string getTypeId() const;
		DMBModel* getDataModel();
		const DMBModel* getDataModel() const;

	protected:
		// Protected data interface
		vl::Var& getData();
		virtual vl::Var& getChildData(const VLVarModel* childPtr);
		void setDataModel(DMBModel* m);

	public:
		// Public Qt model interface
		const VLVarModel* getParentModel() const;
		VLVarModel* getParentModel();
		int getIndex() const;
		virtual int getChildIndex(const VLVarModel* childPtr) const;
		virtual bool removeChild(const VLVarModel* childPtr);

	public:
		// Properties
		Q_PROPERTY (QString id READ id WRITE setId NOTIFY idChanged)
		Q_PROPERTY (QString name READ id WRITE setId NOTIFY idChanged) // Alias for id property
		Q_PROPERTY (ObjectProperty::Type type READ type WRITE setType NOTIFY typeChanged)
		Q_PROPERTY (QVariant value READ value WRITE setValue NOTIFY valueChanged)
		Q_PROPERTY (QVariant valueStr READ valueStr NOTIFY valueChanged)
		Q_PROPERTY (QVariant typeStr READ typeStr NOTIFY typeChanged)
		Q_PROPERTY (QObject* owner READ getDataModel NOTIFY ownerChanged)
		Q_INVOKABLE bool remove();

		QString id() const;
		bool setId(const QString &newId);
		bool setId(const std::string &newId);
		ObjectProperty::Type type() const;
		QString typeStr() const;
		void setType(const ObjectProperty::Type &newType);
		QVariant value() const;
		bool setValue(const QVariant &newValue);
		QVariant valueStr() const;

	private:
		bool setValueInternal(const QVariant &newValue, bool emitValueChanged);

	signals:
		void idChanged(int index) const;
		void typeChanged(int index) const;
		void valueChanged(int index) const;
		void ownerChanged(dmb::DMBModel* newOwner) const;
		void beforeRemove() const;

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
