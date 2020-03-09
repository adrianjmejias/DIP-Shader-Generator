#pragma once
#include "../Filter.h"


namespace ED
{
	class LaplacianOfGaussian :
		public Filter, GPUFilter, CPUFilter
	{
	public:

		// Inherited via GPUFilter
		virtual RawData* ApplyGPU(PARAMS_CONV) override;


		// Inherited via CPUFilter
		virtual RawData* ApplyCPU(PARAMS_CONV) override;

	};
}
