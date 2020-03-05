#pragma once
#include "../Filter.h"

namespace ED {
	class BlackAndWhite : CPUFilter, GPUFilter
	{
	public:
		float threshold = 255*0.6f;


		RawData* ApplyCPU(PARAMS_CONV) override;
		RawData* ApplyGPU(PARAMS_CONV) override;
	};

}
