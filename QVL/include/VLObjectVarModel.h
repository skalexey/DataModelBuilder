#ifndef OBJECT_H
#define OBJECT_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <memory>
#include <string>
#include "OwnPropsCollectionModel.h"
#include "AllPropsCollectionModel.h"
#include "vl_fwd.h"
#include "ModelsFwd.h"
#include "VLVarModel.h"
#include "ObjectProperty.h"


namespace dmb
{
	class VLObjectVarModel : public VLVarModel
	{
		typedef VLVarModel Base;

		friend class VLCollectionModel;
		friend class VLListModel;
		friend class VLVarModel;

		Q_OBJECT

	public:
		// Constructors and initializers
		explicit VLObjectVarModel(QObject* parent = nullptr);
		VLObjectVarModel(const vl::Var& v, QObject* parent = nullptr);
		~VLObjectVarModel();
		void Init(QObject* parent = nullptr) override;
		void Init(QObject *parent, DMBModel* owner) override;
		void Init(QObject *parent, const vl::Var& data, DMBModel* owner) override;
		void Init(const VLVarModelPtr& parent) override;

	public:
		// Type casts
		const VLObjectVarModel* asObject() const override;
		VLObjectVarModel* asObject() override;
		// Type check
		bool isObject() const override;
		// Data interface
		const vl::Object& getData() const;

	public:
		// ====== Public Qt model interface ======
		const VLVarModelPtr& getModel(const std::string& propId) const;
		// Non-const version of getModel
		const VLVarModelPtr& model(const std::string& propId);
		const VLVarModelPtr& setModel(const std::string& propId, const VLVarModelPtr& modelPtr);
		const std::string* getChildId(const VLVarModel* childPtr) const;
		bool removeChild(const VLVarModel* childPtr) override;
		const VLVarModelPtr& getChild(const VLVarModel* p) const override;
		// Other
		bool loadPropList();
		bool setPrototype(const std::string& protoId);
		bool setChildType(const VLVarModel* child, const ObjectProperty::Type &newType) override;
		virtual const vl::Var& setChildValue(const VLVarModel* child, const vl::VarPtr& value) override;

	public:
		// Used by Qt interface
		VLCollectionModel *propListModel();
		QVariant protoPropListModel();
		QVariant allPropListModel();
		QVariant protoId() const;
		QVariant typeId() const;

		const VLCollectionModel& getAllPropsListModel() const {
			static VLCollectionModel emptyModel;
			return mAllPropsListModel ? *mAllPropsListModel : emptyModel;
		}
		VLCollectionModel& getAllPropsListModel() {
			static VLCollectionModel emptyModel;
			return mAllPropsListModel ? *mAllPropsListModel : emptyModel;
		}
		const VLCollectionModel& getOwnPropsListModel() const {
			static VLCollectionModel emptyModel;
			return mOwnPropsListModel ? *mOwnPropsListModel : emptyModel;
		}
		VLCollectionModel& getOwnPropsListModel() {
			static VLCollectionModel emptyModel;
			return mOwnPropsListModel ? *mOwnPropsListModel : emptyModel;
		}
	public:
		// Qt interface
		Q_PROPERTY (VLCollectionModel* propListModel READ propListModel NOTIFY propListChanged)
		Q_PROPERTY (QVariant protoPropListModel READ protoPropListModel NOTIFY protoPropListChanged)
		Q_PROPERTY (QVariant protoId READ protoId NOTIFY protoIdChanged)
		Q_PROPERTY (QVariant typeId READ typeId NOTIFY typeIdChanged)
		Q_PROPERTY (QVariant allProps READ allPropListModel NOTIFY allPropListChanged)
		// New item will be created. If the passed name exists then a number postfix will be added
		Q_INVOKABLE void add(const QString& propId, ObjectProperty::Type type);
		Q_INVOKABLE bool removeProp(const QString& propName);
		Q_INVOKABLE bool removeAt(int index);
		Q_INVOKABLE bool has(const QString& propId) const;
		Q_INVOKABLE bool hasOwn(const QString& propId) const;
		Q_INVOKABLE QVariant at(int index);
		Q_INVOKABLE QVariant get(const QString& propId);
		Q_INVOKABLE QVariant getOwn(const QString& propId);
		Q_INVOKABLE QVariant set(const QString& propId, const QVariant& data);
		// Creates an instance of a type typeId with name instanceName
		// If instanceName is not set then instantiateRequested signal with generated suggested name is called
		Q_INVOKABLE void instantiate(const QString& typeId, const QString& instanceName = "");
		Q_INVOKABLE bool setPrototype(const QVariant& arg);
		Q_INVOKABLE int sizeOwn() const;
		Q_INVOKABLE int sizeAll() const;
		Q_INVOKABLE QString freeId(const QString& desiredId) const;

	signals:
		void nameChanged();
		void propListChanged();
		void protoPropListChanged();
		void instantiateRequested(const QString& instId, const QString& protoId);
		void instantiateRefused(const QString& error);
		void instantiated(const QString& instId, const QString& protoId);
		void protoIdChanged() const;
		void typeIdChanged() const;
		void propAdded(const QString& propId);
		void propRemoved(const QString& propId);
		//void allProtoPropListChanged();
		void allPropListChanged();

	protected:
		// Protected data interface
		vl::Object& data();
		bool has(const std::string& propId) const;
		bool hasOwn(const std::string& propId) const;
		const std::string* getId(int index) const {
			return getOwnPropsListModel().getId(index);
		}
		std::string getFreeId(const std::string& desiredId) const;
		bool renameProperty(const std::string& propId, const std::string& newId);
		const vl::Var& getChildData(const VLVarModel* childPtr) const override;

	private:
		// Data
		std::unique_ptr<AllPropsCollectionModel> mAllPropsListModel;
		std::unique_ptr<OwnPropsCollectionModel> mOwnPropsListModel;
	};
	typedef std::shared_ptr<VLObjectVarModel> VLObjectVarModelPtr;
}

Q_DECLARE_METATYPE(dmb::VLObjectVarModel)

#endif // OBJECT_H
