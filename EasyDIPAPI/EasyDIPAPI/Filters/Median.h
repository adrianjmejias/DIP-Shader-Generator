#pragma once
#include "../Filter.h"


namespace ED
{
	class Median :
		public Filter, CPUFilter, GPUFilter
	{
	public:
		RawData* ApplyGPU(PARAMS_CONV) override;
		RawData* ApplyCPU(PARAMS_CONV) override;
	};
}