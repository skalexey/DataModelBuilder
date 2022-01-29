#ifndef DMBModel_H
#define DMBModel_H

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
		Q_INVOKABLE VLVarModel* addToContent(const QString& instId, const QString& protoId);
		Q_INVOKABLE bool store(const QString& filePath, bool pretty = true);
		Q_INVOKABLE bool load(const QString& filePath);

	protected:
		VLObjectVarModelPtr mRoot = nullptr;
		dmb::Model mDataModel;

	signals:
		void contentModelChanged();
		void typesModelChanged();
		void instantiateRequested(const QString& instId, const QString& protoId);
		void instantiateRefused(const QString& error);
		void modelLoaded(const QString& filePath);
	};
}

#endif // DMBModel_H
