#include "VLUtility.h"
#include "JSONConverter.h"

namespace vl
{
	std::string VarToJSON(const vl::Var& v, const vl::CnvParams& params)
	{
		vl::JSONConverter converter;
		if (v.is<vl::Object>())
			return converter.JSONStr(v.as<vl::Object>(), TypeResolver(), params);
		else
		{
			return "<not supported now>";
		}
	}
}
