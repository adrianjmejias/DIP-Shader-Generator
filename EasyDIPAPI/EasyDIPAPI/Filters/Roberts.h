#pragma once
#include "../Filter.h"

namespace ED
{

	class Roberts : public Filter, GPUFilter, CPUFilter
	{
	public:

		RawData* ApplyCPU(PARAMS_CONV) override;
		RawData* ApplyGPU(PARAMS_CONV) override;
	};

}
