#pragma once
#include "../Filter.h"

namespace ED
{

	class Prewitt : public Filter, GPUFilter, CPUFilter
	{
	public:

		RawData* ApplyCPU(PARAMS_CONV) override;
		RawData* ApplyGPU(PARAMS_CONV) override;
	};

}
