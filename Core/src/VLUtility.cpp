#include "VLUtility.h"
#include "JSONConverter.h"

namespace vl
{
	std::string VarToJSON(const vl::Var& v, const vl::CnvParams& params)
	{
		vl::JSONConverter converter;
		if (v.IsObject())
			return converter.JSONStr(v.AsObject(), TypeResolver(), params);
		else
		{
			return "<not supported now>";
		}
	}
}
