#include "Median.h"


namespace ED
{

RawData* ED::Median::ApplyGPU(PARAMS_CONV)
{
	Kernel& sX = kernels[0];

	FixPadding();

	int szMedian = sX.width * sX.height;
	std::string sobel = BuildShaderConv(
		//uniforms
		R"(
		void swap(inout float xp, inout float yp)
		{
			float temp = xp;
			xp = yp;
			yp = temp;
		}

		// A function to implement bubble sort

)"

"vec3 bubbleSort(vec3 arr[" + std::to_string(szMedian) + "])\n"
R"(
	{
			int i, j;
			int n = arr.length();
			for (i = 0; i < n - 1; i++)

				// Last i elements are already in place  
				for (j = 0; j < n - i - 1; j++)
				{
					if (arr[j].x > arr[j + 1].x)
						swap(arr[j].x, arr[j + 1].x);
					if (arr[j].y > arr[j + 1].y)
						swap(arr[j].y, arr[j + 1].y);
					if (arr[j].z > arr[j + 1].z)
						swap(arr[j].z, arr[j + 1].z);
				}

		return arr[int(arr.length()/2.f)];
		}

		)" +
		UseGradient(),

		//before
		"vec3 medianData[ " + std::to_string(sX.width * sX.height) + " ];"
		//BuildConvolution(sX.values, "convX") +
		//"vec3 avgX = vec3(0);\n"

		,
		// op
		UseForConv(sX.width, sX.height, sX.pivotX, sX.pivotY, sX.d,
			"vec3 color = texture(tex, nUv).rgb;\n"
			"medianData[convI] = color;\n"
			//"avgX +=  color * convX[convI];\n"
		)
		,

		// after

		"fragColor.rgb = bubbleSort(medianData);\n"
		//"fragColor = vec4(medianData[int(medianData.length()/2.f)], 1);\n"
	);


	auto sobelShader(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), sobel.c_str()));




	return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *sobelShader);
}


RawData* Median::ApplyCPU(PARAMS_CONV)
{
	Kernel& sX = kernels[0];
	sX.ApplyPadding();



	std::vector<float> md[4];
	int sz = sX.width * sX.height +2;
	int actSz;
	for (int ii = 0; ii < 4; ii++)
	{
		md[ii].reserve(sz);
		for (size_t jj = 0; jj < sz; jj++)
		{
			md[ii].emplace_back();
		}
	}
	

	return ForEachConvolution(imgData, imgWidth, imgHeight, nChannels, sX.width, sX.height, sX.pivotX, sX.pivotY,
		[&]() {
			actSz = 0;
		},
		[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				md[ii][ic] = src[ii]; 
				//src[ii]++;
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
