#include "Box.h"
namespace ED
{
	RawData* Box::ApplyGPU(PARAMS_CONV)
	{

		Kernel& sX = kernels[0];

		FixPadding();


		std::string sobel = BuildShaderConv(
			//uniforms
			UseGradient(),

			//before
			BuildConvolution(sX.values, "convX") +
			"vec3 avgX = vec3(0);\n"
			,
			// op
			
			
			UseForConv(sX.width, sX.height, sX.pivotX, sX.pivotY, sX.d,
				"vec3 color = texture(tex, nUv).rgb;\n"
				"avgX +=  color * convX[convI];\n"
			)
			,

			// after
			"fragColor = vec4(avgX, 1);\n"
		);


		auto sobelShader(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), sobel.c_str()));




		return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *sobelShader);
	}

	RawData* Box::ApplyCPU(PARAMS_CONV)
	{
		Kernel& sX = kernels[0];
		sX.ApplyPadding();
		float acumX[4];
		return ForEachConvolution(imgData, imgWidth, imgHeight, nChannels, sX.width, sX.height, sX.pivotX, sX.pivotY,
			[&]() {
				Assign<float>(acumX, nChannels, 0.f);
			},
			[&](RawData* src, int ix, int iy, int ic) {
				for (int ii = 0; ii < nChannels; ii++)
				{
					acumX[ii] += src[ii] * sX.values[ic];
				}
			},
				[&](RawData* dest) {
				for (int ii = 0; ii < nChannels; ii++)
				{
					dest[ii] = clamp(255, 0, acumX[ii]);
				}
			});
	}

}
