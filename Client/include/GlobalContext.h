#ifndef GLOBALCONTEXT_H
#define GLOBALCONTEXT_H

#include <functional>

namespace dmb
{
	class Model;
	class DMBModel;

	class GlobalContext
	{
		// TODO: support multiple models
		// ====== User interface =======
	public:
		dmb::Model& GetCurrentDataModel();
		void SetCurrentDataModel(dmb::Model* model);
		DMBModel* GetCurrentModel();
		void SetCurrentModel(DMBModel* model);

	protected:
		// TODO: move to DMBModel and store here only the pointer
		dmb::Model* mCurrentDataModel = nullptr;
		DMBModel* mCurrentModel = nullptr;
		// ====== End of User interface =======
		// ====== Singletone interface =======
	public:
		static GlobalContext& Instance();
		// Support inheritance
		template <typename T>
		void Choose() {
			mCreator = [] {
				return new T;
			};
		}
	protected:
		GlobalContext() = default;

	protected:
		static GlobalContext* mInstance;
		static std::function<GlobalContext*()> mCreator;
	// ====== End of Singletone interface =======
	};
}

#endif // GLOBALCONTEXT_H
