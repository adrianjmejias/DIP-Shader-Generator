#pragma once
#include "../Filter.h"

namespace ED
{


	class ToonShading : Filter, GPUFilter
	{
	public:


		RawData* ApplyGPU(PARAMS_CONV) override;
	};
}