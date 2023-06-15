#ifndef VLLISTVARMODEL_H
#define VLLISTVARMODEL_H

#include <utility>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <QObject>
#include <QString>
#include <QDebug>
#include "VLCollectionModel.h"
#include "vl.h"
#include "VLListModel.h"
#include "VLVarModel.h"

namespace dmb
{
	class VLVarModel;

	class VLListVarModel : public VLVarModel
	{
		typedef VLVarModel Base;

		friend class VLListModel;

		Q_OBJECT

	public:
		// Constructors and initializers
		typedef std::function<const std::string&()> FGetId;
		explicit VLListVarModel(QObject* parent = nullptr);
		VLListVarModel(const vl::Var& v, QObject* parent = nullptr);
		~VLListVarModel();
		void Init(QObject* parent = nullptr) override;
		void Init(QObject *parent, DMBModel* owner) override;
		void Init(const VLVarModelPtr& parent) override;

	public:
		// Type casts
		const VLListVarModel* asList() const override;
		VLListVarModel* asList() override;
		// Type checks
		bool isList() const override;

	public:
		// Public Qt model interface
		const VLListModel &getListModel() const {
			static VLListModel emptyModel;
			return mListModel ? *mListModel : emptyModel;
		}

		VLListModel &getListModel() {
			static VLListModel emptyModel;
			return mListModel ? *mListModel : emptyModel;
		}

		const VLVarModelPtr& getModelAt(int index) const;
		// Non-const version of getModelAt
		const VLVarModelPtr& modelAt(int index);
		int getChildIndex(const VLVarModel* childPtr) const;
		const VLVarModelPtr& addModel(const VLVarModelPtr& modelPtr, int indexBefore = -1);
		bool removeChild(const VLVarModel* childPtr) override;
		const VLVarModelPtr& getChild(const VLVarModel* p) const override;
		const vl::Var& setChildValue(const VLVarModel* child, const vl::VarPtr& value) override;

	public:
		// Properties
		Q_INVOKABLE QVariant at(int index);
		Q_INVOKABLE QVariant add(ObjectProperty::Type type, int indexBefore = -1);
		Q_INVOKABLE QVariant add(const QVariant& data, int indexBefore = -1);
		Q_INVOKABLE bool removeAt(int index);
		Q_INVOKABLE QVariant find(const QVariant& data);
		// Creates an instance of a type typeId
		Q_INVOKABLE void instantiate(const QString& typeId);
		Q_PROPERTY (QVariant listModel READ listModel NOTIFY listChanged)
		Q_INVOKABLE int size() const;

	signals:
		void listChanged();
		void instantiated(const QString& instId);
		void instantiateRefused(const QString& error);

	protected:
		// For Qt interface
		QVariant listModel();
		QVariant add(dmb::VLVarModel* model, int indexBefore = -1);
		// Protected data interface
		vl::List& data();
		vl::Var& data(int index);
		const vl::List& getData() const;
		const vl::Var& getData(int index) const;
		const vl::Var& getChildData(const VLVarModel* childPtr) const override;
		// Other
		bool loadElements();

	private:
		// Data
		std::unique_ptr<VLListModel> mListModel = nullptr;
	};
	typedef std::shared_ptr<VLListVarModel> VLListVarModelPtr;
}

Q_DECLARE_METATYPE(dmb::VLListVarModel)
#endif // VLLISTVARMODEL_H
