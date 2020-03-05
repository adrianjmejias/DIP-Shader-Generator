#pragma once
#include "../Filter.h"

namespace ED
{
	class Negative :
		public Filter, CPUFilter, GPUFilter
	{
	public:
		// Inherited via CPUFilter
		virtual RawData* ApplyCPU(PARAMS_CONV) override;

		// Inherited via GPUFilter
		virtual RawData* ApplyGPU(PARAMS_CONV) override;
	};


}
