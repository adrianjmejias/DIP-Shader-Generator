#pragma once
#include "EDpch.h"
#include "Convolutions.h"
namespace ED {

	RawData* ApplySobelHA(PARAMS_LOCAL);
	RawData* ApplyNegativeHA(PARAMS_GLOBAL);



	RawData* ApplyGreyHA(RawData* data, unsigned int, unsigned int, unsigned int);
	RawData* ApplyBWHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);

	//RawData* ApplyRobertsHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyPrewittHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyBoxHA(RawData* data, unsigned int width, unsigned int height, int nChannels,
		//unsigned int top, unsigned int right, unsigned int bottom, unsigned int left, unsigned int pivotX, unsigned int pivotY);
	//RawData* ApplyMedianHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyLaplaceGaussHA(RawData*, unsigned int, unsigned int);

	bool EDInitGPU();

}