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
#include "VLListModel.h"

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
		~VLListVarModel();
		void Init(QObject* parent = nullptr) override;
		void Init(QObject *parent, const vl::Var& data) override;
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
		vl::List& getData();
		vl::Var& getData(int index);

	public:
		// Public Qt model interface
		const VLVarModel* getAt(int index) const;
		const VLListModel *listModel() const;
		int getChildIndex(const VLVarModel* childPtr) const override;

	protected:
		// Protected Qt model interface
		VLListModel *listModel();
		VLVarModel* getAt(int index);

	protected:
		// Other
		bool loadElements();

	public:
		// Properties
		Q_INVOKABLE void add(ObjectProperty::Type type);
		Q_INVOKABLE bool removeAt(int index);
		Q_INVOKABLE QObject* at(int index);
		Q_PROPERTY (VLListModel* listModel READ listModel NOTIFY listChanged)

	signals:
		void listChanged();

	private:
		// Data
		VLListModel mListModel;
	};
}

Q_DECLARE_METATYPE(dmb::VLListVarModel)
#endif // VLLISTVARMODEL_H
