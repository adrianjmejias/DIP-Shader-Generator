#include "Prewitt.h"


namespace ED
{
	RawData* Prewitt::ApplyCPU(PARAMS_CONV)
	{

		Kernel& sX = kernels[0];
		Kernel& sY = kernels[1];
		FixPadding();

		float acumX[4];
		float acumY[4];
		return ForEachConvolution(imgData, imgWidth, imgHeight, nChannels, sX.width, sX.height, sX.pivotX, sX.pivotY,
			[&]() {
				Assign<float>(acumX, nChannels, 0.f);
				Assign<float>(acumY, nChannels, 0.f);
			},
			[&](RawData* src, int ix, int iy, int ic) {
				for (int ii = 0; ii < nChannels; ii++)
				{
					acumX[ii] += src[ii] * sX.values[ic];
					acumY[ii] += src[ii] * sY.values[ic];
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

	RawData* Prewitt::ApplyGPU(PARAMS_CONV)
	{

		Kernel& sX = kernels[0];
		Kernel& sY = kernels[1];

		FixPadding();


		std::string sobel = BuildShaderConv(
			//uniforms
			UseGradient(),

			//before
			BuildConvolution(sX.values, "convX") +
			BuildConvolution(sY.values, "convY") +
			"vec3 avgX = vec3(0);\n"
			"vec3 avgY = vec3(0);\n"
			,
			// op
			UseForConv(sX.width, sX.height, sX.pivotX, sX.pivotY, sX.d,
				"vec3 color = texture(tex, nUv).rgb;\n"
				"avgX +=  color * convX[convI];\n"
			) +
			UseForConv(sY.width, sY.height, sY.pivotX, sY.pivotY, sY.d,
				"vec3 color = texture(tex, nUv).rgb;\n"
				"avgY +=  color * convY[convI];\n"
			)
			,

			// after
			"fragColor = vec4(SU_GRADIENT(avgX, avgY), 1);\n"//, sY.width, sY.height, sY.pivotX, sY.pivotY
		);


		auto sobelShader(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), sobel.c_str()));




		return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *sobelShader);
	}
}