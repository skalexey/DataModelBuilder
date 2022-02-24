#ifndef VLLISTVARMODEL_H
#define VLLISTVARMODEL_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include "VLCollectionModel.h"
#include "vl.h"
#include "VLListModel.h" // No forward def instead because of Qt meta object resolution
#include "VLVarModel.h"

namespace dmb
{
	class VLVarModel;

	class VLListVarModel : public VLVarModel, public std::enable_shared_from_this<VLListVarModel>
	{
		typedef VLVarModel Base;

		Q_OBJECT

	public:
		// Constructors and initializers
		typedef std::function<const std::string&()> FGetId;
		explicit VLListVarModel(QObject* parent = nullptr);
		VLListVarModel(const vl::Var& v, QObject* parent = nullptr);
		~VLListVarModel();
		void Init(QObject* parent = nullptr) override;
		void Init(QObject *parent, const vl::Var& data, DMBModel* owner) override;
		void Init(const VLVarModelPtr& parent) override;

	public:
		// Type casts
		const VLListVarModel* asList() const override;
		VLListVarModel* asList() override;
		// Type checks
		bool isList() const override;

	public:
		// Public data interface
		const vl::List& getData() const;
		const vl::Var& getData(int index) const;
		const vl::Var& getChildData(const VLVarModel* childPtr) const override;

	protected:
		// Protected data interface
		vl::List& data();
		vl::Var& data(int index);

	public:
		// Public Qt model interface
		const VLVarModel* getAt(int index) const;
		const VLListModel *getListModel() const;
		const VLVarModelPtr& getAtSp(int index) const;
		const VLVarModelPtr& atSp(int index);
		VLListModel *listModel();
		int getChildIndex(const VLVarModel* childPtr) const override;
		VLVarModel* add(const dmb::VLVarModel* model, int indexBefore = -1);
		const VLVarModelPtr& addModel(const VLVarModelPtr& modelPtr, int indexBefore = -1);
		bool removeChild(const VLVarModel* childPtr) override;
		const VLVarModelPtr& getChildPtr(const VLVarModel* p) const override;
		VLVarModelPtr getPtr() override;

	protected:
		// Protected Qt model interface

	protected:
		// Other
		bool loadElements();

	public:
		// Properties
		Q_INVOKABLE dmb::VLVarModel* at(int index);
		Q_INVOKABLE dmb::VLVarModel* add(ObjectProperty::Type type, int indexBefore = -1);
		Q_INVOKABLE dmb::VLVarModel* add(const QVariant& data, int indexBefore = -1);
		Q_INVOKABLE bool removeAt(int index);
		Q_INVOKABLE dmb::VLVarModel* find(const QVariant& data);
		// Creates an instance of a type typeId
		Q_INVOKABLE void instantiate(const QString& typeId);
		Q_PROPERTY (VLListModel* listModel READ listModel NOTIFY listChanged)
		Q_INVOKABLE int size() const;

	signals:
		void listChanged();
		void instantiated(const QString& instId);
		void instantiateRefused(const QString& error);

	private:
		// Data
		VLListModel mListModel;
	};
	typedef std::shared_ptr<VLListVarModel> VLListVarModelPtr;
}

Q_DECLARE_METATYPE(dmb::VLListVarModel)
#endif // VLLISTVARMODEL_H
