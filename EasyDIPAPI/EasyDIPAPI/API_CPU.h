#pragma once
#include "EDpch.h"

namespace ED {

	RawData* ApplyNegative(PARAMS_GLOBAL);
	RawData* ApplySobel(PARAMS_LOCAL);



	RawData* ApplyGrey(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);
	RawData* ApplyBW(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels);


RawData* ApplyRoberts(RawData* data, unsigned int width, unsigned int height, int nChannels,
	unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

RawData* ApplyPrewitt(RawData* data, unsigned int width, unsigned int height, int nChannels,
	unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

RawData* ApplyBox(RawData* data, unsigned int width, unsigned int height, int nChannels,
	unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

RawData* ApplyMedian(RawData* data, unsigned int width, unsigned int height, int nChannels,
	unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

RawData* ApplyLaplaceGauss(RawData* data, unsigned int width, unsigned int height, int nChannels,
	unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY);

}