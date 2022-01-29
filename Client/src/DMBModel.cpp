#include "DMBModel.h"
#include "DMBCore.h"
#include "GlobalContext.h"
#include "Utils.h"
#include "VLObjectVarModel.h"

namespace dmb
{
	DMBModel::DMBModel(QObject *parent)
		: QObject(parent)
	{
		mRoot = std::make_shared<VLObjectVarModel>();
		mRoot->Init(nullptr, getDataModel().GetData());
		// TODO: support multiple models
		GlobalContext::Instance().SetCurrentModel(this);
		GlobalContext::Instance().SetCurrentDataModel(&mDataModel);
	}

	DMBModel::~DMBModel()
	{}

	const VLObjectVarModel *DMBModel::getContentModel() const
	{
		// Invoke const version of getModel
		if (auto model = mRoot->getModel("content"))
			return model->asObject();
		return nullptr;
	}

	VLObjectVarModel *DMBModel::contentModel()
	{
		// Invoke non-const version of getModel
		if (auto model = mRoot->getModel("content"))
			return model->asObject();
		return nullptr;
	}

	const VLObjectVarModel *DMBModel::getTypesModel() const
	{
		// Invoke const version of getModel
		if (auto model = mRoot->getModel("types"))
			return model->asObject();
		return nullptr;
	}

	VLObjectVarModel *DMBModel::typesModel()
	{
		// Invoke non-const version of getModel
		if (auto model = mRoot->getModel("types"))
			return model->asObject();
		return nullptr;
	}

	const Model &DMBModel::getDataModel() const
	{
		return mDataModel;
	}

	Model &DMBModel::getDataModel()
	{
		return mDataModel;
	}

	void DMBModel::instantiateRequest(int index)
	{
		if (auto m = typesModel())
			if (index >= 0 && index < m->propListModel()->size())
			{
				const auto& id = m->getId(index);
				auto instId = id;
				auto& content = getDataModel().GetContent();
				bool success = true;
				if (content.Has(instId))
				{
					std::string tpl = instId;
					for (int i = 1; i < std::numeric_limits<short>::max(); i++)
						if ((success = !content.Has(instId = Utils::FormatStr("%s %d", tpl.c_str(), i))))
							break;
				}
				if (success)
					emit instantiateRequested(QString(instId.c_str()), QString(id.c_str()));
				else
					emit instantiateRefused("Name error");
			}
	}

	VLVarModel* DMBModel::addToContent(const QString &instId, const QString &qProtoId)
	{
		if (auto m = contentModel())
		{
			auto protoId = qProtoId.toStdString();
			if (auto proto = const_cast<const DMBModel*>(this)->
					getTypesModel()->getData().Get(protoId).AsObject())
			{
				vl::Object o;
				o.SetPrototype(proto);
				m->propListModel()->setData(instId.toStdString(), o);
				if (auto newModel = m->get(instId))
					return newModel;
			}
		}
		return nullptr;
	}

	bool DMBModel::store(const QString &filePath, bool pretty)
	{
		return getDataModel().Store(filePath.toStdString(), { pretty });
	}

	bool DMBModel::load(const QString &filePath)
	{
		if (getDataModel().Load(filePath.toStdString()))
		{
			emit modelLoaded(filePath);
			return true;
		}
		return false;
	}
}
