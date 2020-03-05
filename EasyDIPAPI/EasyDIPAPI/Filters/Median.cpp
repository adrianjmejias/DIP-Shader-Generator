#include "Median.h"


namespace ED
{

RawData* ED::Median::ApplyGPU(PARAMS_CONV)
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


RawData* Median::ApplyCPU(PARAMS_CONV)
{
	Kernel& sX = kernels[0];
	sX.ApplyPadding();



	float* md[4];
	int sz = sX.width * sX.height;
	int actSz;

	{
		int bsz = sizeof(float) * sz;
		md[0] = (float*)alloca(bsz);
		md[1] = (float*)alloca(bsz);
		md[2] = (float*)alloca(bsz);
		md[3] = (float*)alloca(bsz);
	}

	return ForEachConvolution(imgData, imgWidth, imgWidth, nChannels, sX.width, sX.height, sX.pivotX, sX.pivotY,
		[&]() {
			actSz = 0;
		},
		[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				md[ii][ic] = src[ii];
			}
			actSz++;
		},
			[&](RawData* dest) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				std::sort(&md[ii][0], &md[ii][actSz]);
				dest[ii] = clamp(255, 0, md[ii][actSz / 2]);
			}
		}
		);
}
}
