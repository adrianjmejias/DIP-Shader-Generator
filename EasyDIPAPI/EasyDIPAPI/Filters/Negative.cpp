#include "Negative.h"
namespace ED
{
	RawData* ED::Negative::ApplyCPU(PARAMS_CONV)
	{
		return ForEachPixel(imgData, imgWidth, imgHeight, nChannels, [](RawData* dest, RawData* src) {
			dest[0] = 1 - src[0];
			dest[1] = 1 - src[1];
			dest[2] = 1 - src[2];
		});
	}

	RawData* Negative::ApplyGPU(PARAMS_CONV)
	{
		std::string shader = BuildGlobalShader(

			"fragColor = vec4(1- fragColor.rgb , 1);\n"
			,
			""
		);


		auto shaderProgram(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), shader.c_str()));

		return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *shaderProgram);
	}

}
