#ifndef DMBModel_H
#define DMBModel_H

#include <unordered_map>
#include <QtQml/qqml.h>
#include <QObject>
#include "DMBCore.h"
#include "ModelsFwd.h"
#include "VLObjectVarModel.h"
// Needed for Q_INVOKABLE dmb::VLListVarModel*
#include "VLListVarModel.h"

namespace dmb
{
	class Model;
	class ModelStorage;

	class DMBModel : public QObject
	{
		friend class VLVarModel;
		friend class ModelStorage;

		Q_OBJECT
		QML_NAMED_ELEMENT(DMBModel)

	public:
		explicit DMBModel(QObject *parent = nullptr);
		~DMBModel();
		// Public interface
		VLObjectVarModel* contentModel();
		const VLObjectVarModel* getContentModel() const;
		VLObjectVarModel* typesModel();
		const VLObjectVarModel* getTypesModel() const;
		const dmb::Model& getDataModel() const {
			return mDataModel;
		}
		dmb::Model& getDataModel() {
			return mDataModel;
		}
		bool isLoaded() const;
		const VLVarModelPtr& createFromDataPtr(const QVariant& data);
		dmb::VLObjectVarModelPtr createObjectSp();
		dmb::VLListVarModelPtr createListSp();
		VLVarModelPtr takeStandaloneModel(const VLVarModel* p);
		bool hasStandaloneModel(const VLVarModel* p) {
			return mStandaloneModels.find(p) != mStandaloneModels.end();
		}
		bool removeStandaloneModel(const VLVarModel* p);
		QString currentFile() const;
		const std::string& getCurrentFile() const;
		bool setLoadFrom(const QString& filePath);
		const VLVarModelPtr& modelByTypeId(const std::string& typeId);
		// Properties
		Q_PROPERTY (VLObjectVarModel* contentModel READ contentModel NOTIFY contentModelChanged)
		Q_PROPERTY (VLObjectVarModel* typesModel READ typesModel NOTIFY typesModelChanged)
		Q_INVOKABLE bool store(const QString& filePath = "", bool pretty = true);
		Q_INVOKABLE bool storeContent(const QString& filePath = "", bool pretty = true);
		Q_INVOKABLE bool load(const QString& filePath);
		Q_PROPERTY (bool isLoaded READ isLoaded NOTIFY modelLoaded);
		Q_INVOKABLE dmb::VLVarModel* createFromData(const QVariant& data);
		Q_INVOKABLE dmb::VLObjectVarModel* createObject();
		Q_INVOKABLE dmb::VLListVarModel* createList();
		Q_PROPERTY (QString currentFile READ currentFile NOTIFY currentFileChanged)
		Q_PROPERTY (QString loadFrom WRITE setLoadFrom)
		Q_INVOKABLE void clear();
		Q_INVOKABLE bool hasChanges() const;

	protected:
		const VLVarModelPtr &storeStandaloneModel(const VLVarModelPtr& ptr);
		void setCurrentFile(const std::string& newFilePath);
		const VLObjectVarModelPtr& getRoot() {
			return mRoot;
		}
		void InitRoot();

	protected:
		VLObjectVarModelPtr mRoot = nullptr;
		dmb::Model mDataModel;

		std::string mFilePath;
		std::unordered_map<const VLVarModel*, VLVarModelPtr> mStandaloneModels;

	signals:
		void contentModelChanged();
		void typesModelChanged();
		void modelLoaded(const QString& filePath);
		void modelLoadError(const QString& filePath, const QString& error);
		void modelStored(const QString& filePath);
		void modelStoreError(const QString& filePath, const QString& error);
		void currentFileChanged();
	};
}

#endif // DMBModel_H
