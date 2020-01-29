#pragma once
#include "EDpch.h"
#include "EDImage.h"
namespace ED {


	std::vector<float> Normalize(std::vector<float> d);

	RawData* ApplyConvolutionHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, Shader& s, unsigned int *tex = nullptr);
	RawData* ApplyGreyHA(RawData*, unsigned int, unsigned int, unsigned int);
	//RawData* ApplyBWHA(RawData*, unsigned int, unsigned int);
	RawData* ApplySobelHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	//RawData* ApplyRobertsHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyPrewittHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyBoxHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyMedianHA(RawData*, unsigned int, unsigned int);
	//RawData* ApplyLaplaceGaussHA(RawData*, unsigned int, unsigned int);





	RawData* ApplyGrey(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	//RawData* ApplyBW(RawData*, unsigned int, unsigned int);
	RawData* ApplySobel(RawData* data, unsigned int width, unsigned int height, int nChannels);
	//RawData* ApplyRoberts(RawData*, unsigned int, unsigned int);
	//RawData* ApplyPrewitt(RawData*, unsigned int, unsigned int);
	//RawData* ApplyBox(RawData*, unsigned int, unsigned int);
	//RawData* ApplyMedian(RawData*, unsigned int, unsigned int);
	//RawData* ApplyLaplaceGauss(RawData*, unsigned int, unsigned int);

	bool EDInit();
}