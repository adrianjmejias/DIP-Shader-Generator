#pragma once
#include "Kernel.h"
#include "ShaderUtility.h"


namespace ED
{


	struct CPUFilter
	{
		virtual RawData* ApplyCPU(PARAMS_CONV) = 0;

	};

	struct GPUFilter {
		virtual RawData* ApplyGPU(PARAMS_CONV) = 0;
	};

class Filter
{
public:

	std::vector<Kernel> kernels;

	void FixPadding()
	{
		for(auto &k : kernels)
		{
			k.ApplyPadding();
		}
	}
	bool LoadFromFile(const std::string& path);

};
}
