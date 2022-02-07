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
		const VLVarModel* getModel(const std::string& propId) const;
		const VLVarModel* getAt(int index) const;
		VLVarModel* getModel(const std::string& propId);
		VLVarModel* getAt(int index);
		int getChildIndex(const VLVarModel* childPtr) const override;
		const std::string& getChildId(const VLVarModel* childPtr) const;
		bool removeChild(const VLVarModel* childPtr) override;
		// Other
		bool loadPropList();

	public:
		// Properties
		VLCollectionModel *propListModel();
		VLCollectionModel *protoPropListModel();
		VLCollectionModel& getPropListModel();
		Q_PROPERTY (VLCollectionModel* propListModel READ propListModel NOTIFY propListChanged)
		Q_PROPERTY (VLCollectionModel* protoPropListModel READ protoPropListModel NOTIFY protoPropListChanged)
		// New item will be created. If the passed name exists then a number postfix will be added
		Q_INVOKABLE void add(const QString& propId, ObjectProperty::Type type);
		Q_INVOKABLE bool remove(const QString& propName);
		Q_INVOKABLE bool removeAt(int index);
		Q_INVOKABLE bool has(const QString& propId) const;
		Q_INVOKABLE dmb::VLVarModel* at(int index);
		Q_INVOKABLE dmb::VLVarModel* get(const QString& propId);
		Q_INVOKABLE dmb::VLVarModel* set(const QString& propId, VLVarModel* var);
		Q_INVOKABLE dmb::VLVarModel* set(const QString& propId, const QVariant& data);

	signals:
		void nameChanged();
		void propListChanged();
		void protoPropListChanged();

	private:
		// Data
		VLCollectionModel mPropListModel;
	};
	typedef std::shared_ptr<VLObjectVarModel> VLObjectVarModelPtr;

	extern VLObjectVarModel* emptyObjectModel;
}

Q_DECLARE_METATYPE(dmb::VLObjectVarModel)

#endif // OBJECT_H
