#ifndef DMBModel_H
#define DMBModel_H

#include <unordered_map>
#include <QtQml/qqml.h>
#include <QObject>
#include "DMBCore.h"
#include "ModelsFwd.h"
#include "VLObjectVarModel.h"

namespace dmb
{
	class Model;

	class DMBModel : public QObject
	{
		Q_OBJECT
		QML_NAMED_ELEMENT(DMBModel)

	public:
		explicit DMBModel(QObject *parent = nullptr);
		~DMBModel();
		VLObjectVarModel* contentModel();
		const VLObjectVarModel* getContentModel() const;
		VLObjectVarModel* typesModel();
		const VLObjectVarModel* getTypesModel() const;
		const dmb::Model& getDataModel() const;
		dmb::Model& getDataModel();
		Q_PROPERTY (VLObjectVarModel* contentModel READ contentModel NOTIFY contentModelChanged)
		Q_PROPERTY (VLObjectVarModel* typesModel READ typesModel NOTIFY typesModelChanged)
		Q_INVOKABLE void instantiateRequest(int index);
		Q_INVOKABLE dmb::VLVarModel* addToContent(const QString& instId, const QString& protoId);
		Q_INVOKABLE bool store(const QString& filePath = "", bool pretty = true);
		Q_INVOKABLE bool load(const QString& filePath);
		Q_PROPERTY (bool isLoaded READ isLoaded NOTIFY modelLoaded);
		bool isLoaded() const;
		Q_INVOKABLE dmb::VLVarModel* createFromData(const QVariant& data);
		Q_INVOKABLE dmb::VLVarModel* createObject();
		Q_INVOKABLE dmb::VLVarModel* createList();
		dmb::VLVarModelPtr createPtrFromData(const QVariant& data);
		dmb::VLVarModelPtr createObjectPtr();
		dmb::VLVarModelPtr createListPtr();
		VLVarModelPtr takeStandaloneModel(const VLVarModel* p);
		bool removeStandaloneModel(const VLVarModel* p);
		
	protected:
		const VLVarModelPtr &storeStandalonePtr(const VLVarModelPtr& ptr);

	protected:
		VLObjectVarModelPtr mRoot = nullptr;
		dmb::Model mDataModel;
		std::string mFilePath;
		std::unordered_map<const VLVarModel*, VLVarModelPtr> mStandaloneModels;

	signals:
		void contentModelChanged();
		void typesModelChanged();
		void instantiateRequested(const QString& instId, const QString& protoId);
		void instantiateRefused(const QString& error);
		void modelLoaded(const QString& filePath);
		void modelLoadError(const QString& error);
	};
}

#endif // DMBModel_H
