#pragma once

#include <string>
#include "vl_fwd.h"
#include "JSONDefs.h"

namespace vl
{
	std::string VarToJSON(const vl::Var& v, const vl::CnvParams& params = {true, true, true});
}