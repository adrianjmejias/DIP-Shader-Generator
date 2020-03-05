#pragma once
#include "../Filter.h"

namespace ED{
class Embossing : Filter, GPUFilter
{
	glm::mat4 rotMat;


public:
	float distance = 1;
	glm::vec1 rot{ 0 };

	RawData* ApplyGPU(PARAMS_CONV) override;
};
}