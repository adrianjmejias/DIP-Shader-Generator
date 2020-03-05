#include "Greyscale.h"
namespace ED
{
	RawData* Greyscale::ApplyCPU(PARAMS_CONV) {
		return ForEachPixel(imgData, imgWidth, imgHeight, nChannels, [&](RawData* dest, RawData* src) {
			float c = src[0] * 0.2125f + src[1] * 0.7154f + src[2] * 0.0721f;

			dest[0] = c;
			dest[1] = c;
			dest[2] = c;
			});
	}

	RawData* Greyscale::ApplyGPU(PARAMS_CONV)
	{
		std::string shader = BuildGlobalShader(

			"fragColor = vec4(SU_GREYSCALE(fragColor.rgb), 1);\n"
			,
			UseGreyScale()
		);


		auto shaderProgram(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), shader.c_str()));

		return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *shaderProgram);
	}
}