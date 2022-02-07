#ifndef MODELSFWD_H
#define MODELSFWD_H

#include <memory>

namespace dmb {
	class VLVarModel;
	typedef std::shared_ptr<VLVarModel> VLVarModelPtr;

	class VLObjectVarModel;
	typedef std::shared_ptr<VLObjectVarModel> VLObjectVarModelPtr;

	class VLListVarModel;

	class VLListModelInterface;
	class VLListModel;
	class VLCollectionModel;

	class DMBModel;
}

#endif // MODELSFWD_H
