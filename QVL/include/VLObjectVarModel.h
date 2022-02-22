#ifndef OBJECT_H
#define OBJECT_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <memory>
#include <string>
#include "VLCollectionModel.h"
#include "vl.h"
#include "VLListModel.h" // No forward def instead because of Qt meta object resolution
#include "VLVarModel.h"
#include "ObjectProperty.h"


namespace dmb
{
	class VLObjectVarModel : public VLVarModel, public std::enable_shared_from_this<VLObjectVarModel>
	{
		typedef VLVarModel Base;

		Q_OBJECT

	public:
		// Constructors and initializers
		explicit VLObjectVarModel(QObject* parent = nullptr);
		VLObjectVarModel(const vl::Var& v, QObject* parent = nullptr);
		~VLObjectVarModel();
		void Init(QObject* parent = nullptr) override;
		void Init(QObject *parent, const vl::Var& data, DMBModel* owner) override;
		void Init(const VLVarModelPtr& parent) override;

	public:
		// Type casts
		const VLObjectVarModel* asObject() const override;
		VLObjectVarModel* asObject() override;
		// Type check
		bool isObject() const override;

	public:
		// Public data interface
		const vl::ObjectVar& getData() const;
		bool has(const std::string& propId) const;
		const std::string& getId(int index) const;
		int getIndex(const std::string& propId) const;
		bool renameProperty(const std::string& propId, const std::string& newId);
		const vl::Var& getChildData(const VLVarModel* childPtr) const override;

	protected:
		// Protected data interface
		vl::ObjectVar& getData();
		vl::Var& setData(const std::string& propId, const vl::Var& v);
		const std::string& getPropId(int index) const;

	public:
		// Public Qt model interface
		const VLVarModelPtr& getModelSp(const std::string& propId) const;
		// Non-const version of getModelSp
		const VLVarModelPtr& modelSp(const std::string& propId);
		const VLVarModelPtr& setModel(const std::string& propId, const VLVarModelPtr& modelPtr);
		const VLVarModel* getModel(const std::string& propId) const;
		const VLVarModel* getAt(int index) const;
		VLVarModel* model(const std::string& propId);
		VLVarModel* getAt(int index);
		int getChildIndex(const VLVarModel* childPtr) const override;
		const std::string& getChildId(const VLVarModel* childPtr) const;
		bool removeChild(const VLVarModel* childPtr) override;
		// Other
		bool loadPropList();
		bool setPrototype(const std::string& protoId);

	public:
		// Properties
		VLCollectionModel *propListModel();
		VLCollectionModel *protoPropListModel();
		VLCollectionModel& getPropListModel();
		Q_PROPERTY (VLCollectionModel* propListModel READ propListModel NOTIFY propListChanged)
		Q_PROPERTY (VLCollectionModel* protoPropListModel READ protoPropListModel NOTIFY protoPropListChanged)
		Q_PROPERTY (QVariant protoId READ protoId NOTIFY protoIdChanged)
		Q_PROPERTY (QVariant typeId READ typeId NOTIFY typeIdChanged)
		// New item will be created. If the passed name exists then a number postfix will be added
		Q_INVOKABLE void add(const QString& propId, ObjectProperty::Type type);
		Q_INVOKABLE bool removeProp(const QString& propName);
		Q_INVOKABLE bool removeAt(int index);
		Q_INVOKABLE bool has(const QString& propId) const;
		Q_INVOKABLE dmb::VLVarModel* at(int index);
		Q_INVOKABLE dmb::VLVarModel* get(const QString& propId);
		Q_INVOKABLE dmb::VLVarModel* set(const QString& propId, VLVarModel* var);
		Q_INVOKABLE dmb::VLVarModel* set(const QString& propId, const QVariant& data);
		// Creates an instance of a type typeId with name instanceName
		// If instanceName is not set then instantiateRequested signal with generated suggested name is called
		Q_INVOKABLE void instantiate(const QString& typeId, const QString& instanceName = "");
		Q_INVOKABLE bool setPrototype(const QString& protoId);
		Q_INVOKABLE bool setPrototype(VLObjectVarModel* model);

		QVariant protoId() const;
		QVariant typeId() const;

	signals:
		void nameChanged();
		void propListChanged();
		void protoPropListChanged();
		void instantiateRequested(const QString& instId, const QString& protoId);
		void instantiateRefused(const QString& error);
		void instantiated(const QString& instId, const QString& protoId);
		void protoIdChanged() const;
		void typeIdChanged() const;

	private:
		// Data
		VLCollectionModel mPropListModel;
	};
	typedef std::shared_ptr<VLObjectVarModel> VLObjectVarModelPtr;

	extern VLObjectVarModel* emptyObjectModel;
}

Q_DECLARE_METATYPE(dmb::VLObjectVarModel)

#endif // OBJECT_H
