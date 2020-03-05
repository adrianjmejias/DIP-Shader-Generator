#include "Embossing.h"

namespace ED
{

RawData* ED::Embossing::ApplyGPU(PARAMS_CONV)
{
	rotMat = glm::rotate(glm::mat4(1), rot.x, glm::vec3(0, 0, 1));

	std::string sobel = BuildGlobalShader(
		"ivec2 ires = textureSize(tex, 0);              \n   "
		"float ResS = float(ires.s);					\n		   "
		"float ResT = float(ires.t);					\n		   "
		"vec2 vST = actPos;								\n   "
		"vec3 irgb = texture(tex, vST).rgb;				\n   "
		"vec2 stpp = (vec4(1. / ResS, 1. / ResT,0,1) *rotMat).rg * distance;		\n		   "
		"vec3 c00 = texture(tex, vST).rgb;				\n  "
		"vec3 cp1p1 = texture(tex, vST + stpp ).rgb;		\n  "
		"vec3 diffs = c00 - cp1p1; // vector difference	\n		   "
		"float max = diffs.r;							\n		   "
		"if (abs(diffs.g) > abs(max) ) max = diffs.g;	\n		   "
		"if (abs(diffs.b) > abs(max) ) max = diffs.b;	\n		   "
		"float gray = clamp(max + .5, 0., 1.);			\n		   "
		"vec3 color = vec3(gray, gray, gray);			\n		   "
		"fragColor = vec4(color, 1.);					\n		   "


		//"fragColor = vec4(SU_GRADIENT(avgX, avgY), 1);\n"//, sY.width, sY.height, sY.pivotX, sY.pivotY
		,
		//uniforms
		"uniform mat4 rotMat;\n"
		"uniform float distance;\n" +
		UseGreyScale()
	);


	auto sobelShader(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), sobel.c_str()));
	sobelShader->use();
	sobelShader->setMat4("rotMat", rotMat);
	sobelShader->setFloat("distance", distance);


	return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *sobelShader);
}
}
