#include "API_CPU.h"


namespace ED {
	RawData* ApplySobel(PARAMS_LOCAL) {

		std::vector<float> sobelX = {
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1,
		};
		std::vector<float> sobelY = {
			-1 , 0, 1,
			-2 , 0, 2,
			-1 , 0, 1,
		};

		float acumX[4];
		float acumY[4];
		return ForEachConvolution(data, width, height, nChannels, 3, 3, 1, 1,
			[&]() {
			Assign<float>(acumX, nChannels, 0.f);
			Assign<float>(acumY, nChannels, 0.f);
		},
			[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				acumX[ii] += src[ii] * sobelX[ic];
				acumY[ii] += src[ii] * sobelY[ic];
			}
		},
			[&](RawData* dest) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				float x = acumX[ii] * acumX[ii];
				float y = acumY[ii] * acumY[ii];
				dest[ii] = clamp(255, 0, sqrt(x + y));
			}
		});
	}

	RawData* ApplyNegative(PARAMS_GLOBAL) {
		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			dest[0] = 1 - src[0];
			dest[1] = 1 - src[1];
			dest[2] = 1 - src[2];
		});
	}
}