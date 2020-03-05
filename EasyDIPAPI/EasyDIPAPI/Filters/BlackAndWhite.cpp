#include "BlackAndWhite.h"
namespace ED
{
	RawData* ED::BlackAndWhite::ApplyCPU(PARAMS_CONV) {
		return ForEachPixel(imgData, imgWidth, imgHeight, nChannels, [&](RawData* dest, RawData* src) {
			float c = src[0] * 0.2125f + src[1] * 0.7154f + src[2] * 0.0721f;
			c = (c > threshold) ? 255.f : 0;

			dest[0] = c;
			dest[1] = c;
			dest[2] = c;
			});
	}

	RawData* ED::BlackAndWhite::ApplyGPU(PARAMS_CONV)
	{
		std::string shader = BuildGlobalShader(

			"fragColor = vec4(SU_BW(fragColor.rgb, " + std::to_string(threshold) + "/255.f), 1);\n"
			,
			UseBW()
		);


		auto shaderProgram(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), shader.c_str()));

		return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *shaderProgram);
	}
}