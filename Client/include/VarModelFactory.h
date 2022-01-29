#ifndef VARMODELFACTORY_H
#define VARMODELFACTORY_H

#include <memory>
#include <functional>
#include <QObject>
#include "DMBCore.h"
#include "vl_fwd.h"
#include "ModelsFwd.h"

namespace dmb
{
	class VarModelFactory
	{
		// ====== User interface =======
	public:
		VLVarModelPtr Create(const vl::Var& var);

	protected:
		// ====== End of User interface =======
		// ====== Singletone interface =======
	public:
		static VarModelFactory& Instance();
		// Support inheritance
		template <typename T>
		void Choose() {
			mCreator = [] {
				return new T;
			};
		}
	protected:
		VarModelFactory() = default;

	protected:
		static VarModelFactory* mInstance;
		static std::function<VarModelFactory*()> mCreator;
	// ====== End of Singletone interface =======
	};
}

#endif // VARMODELFACTORY_H
