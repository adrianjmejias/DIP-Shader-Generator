#include "API_CPU.h"


namespace ED {



	RawData* ApplyNegative(PARAMS_GLOBAL) {
		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			dest[0] = 1 - src[0];
			dest[1] = 1 - src[1];
			dest[2] = 1 - src[2];
		});
	}
}