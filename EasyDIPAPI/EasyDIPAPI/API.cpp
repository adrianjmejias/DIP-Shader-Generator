
#include "API.h"

namespace ED {





	//RawData* ApplyGreyHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
	//	return ApplyConvolutionHA(data, width, height, nChannels, *greyShader);
	//}
	//RawData* ApplyBWHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
	//	return ApplyConvolutionHA(data, width, height, nChannels, *bwShader);
	//}
	//RawData* ApplyNegativeHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
	//	return ApplyConvolutionHA(data, width, height, nChannels, *negativeShader);
	//}
	//RawData* ApplyRobertsHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
	//
	//
	//}
	//RawData* ApplyPrewittHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyBoxHA(RawData* data, unsigned int width, unsigned int height, int nChannels,
	//	unsigned int top, unsigned int right, unsigned int bottom, unsigned int left, unsigned int pivotX, unsigned int pivotY) {
	//	
	//	std::vector<float> box = ED::Normalize({
	//		0,-3,-7,-9,-7,-3,0,
	//		-3,-16,-23,-18,-23,-16,-3,
	//		-7,-23,11,65,11,-23,-7,
	//		-9,-18,65,167,65,-18,-9,
	//		-7,-23,11,65,11,-23,-7,
	//		-3,-16,-23,-18,-23,-16,-3,
	//		0,-3,-7,-9,-7,-3,0
	//	});

	//	auto [conv, newWidth, newHeight] = ReduceConvolution(box, 7, 7, top, right, bottom, left);

	//	std::string sobel = BuildShaderConv(
	//		BuildConvolution(conv, "conv") +
	//		//BuildConvolution({0,0,0,0,0,0,0,0,0,1}, "disp") +
	//		"vec3 avgX = vec3(0);\n",

	//		"vec3 color = texture(tex, nUv).rgb;\n"
	//		"avgX +=  color * conv[convI];\n",

	//		"fragColor = vec4(avgX, 1);\n",
	//		UseGradient(),
	//		newWidth, newHeight, 1, 1);


	//	sobelShader.reset(Shader::FromString(vert.c_str(), sobel.c_str()));

	//	return ApplyConvolutionHA(data, width, height, nChannels, *sobelShader);

	//}
	//RawData* ApplyMedianHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyLaplaceGaussHA(RawData* data, unsigned int width, unsigned int height) {}

	//RawData* PreApplyConvolution()
	//{

	//}

	RawData* ApplyGrey(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {

		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			float c = src[0] * 0.2125f + src[1] * 0.7154f + src[2] * 0.0721f;
			dest[0] = c;
			dest[1] = c;
			dest[2] = c;
		});
	}

	RawData* ApplyBW(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			float c = src[0] * 0.2125f + src[1] * 0.7154f + src[2] * 0.0721f;
			c = (c > 0.6) ? 1 : 0;

			dest[0] = c;
			dest[1] = c;
			dest[2] = c;
		});
	}

	RawData* ApplyRoberts(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY) {

		std::vector<float> sobelX = {
			1, 0,
			0, -1,
		};
		std::vector<float> sobelY = {
			0,  1,
			-1,	 0,
		};

		float acumX[4];
		float acumY[4];
		return ForEachConvolution(data, width, height, nChannels, 2, 2, 0, 0,
			[&]() {
			Assign<float>(acumX, nChannels, 0.f);
			Assign<float>(acumY, nChannels, 0.f);
		},
			[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				acumX[ii] += src[ii] * sobelX[ic];
				acumY[ii] += src[ii] * sobelY[ic];
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
	RawData* ApplyPrewitt(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY) {

		std::vector<float> sobelX = {
			1,0,-1,
			1,0,-1,
			1,0,-1,
		};
		std::vector<float> sobelY = {
			1,1,1,
			0,0,0,
			-1,-1,-1,
		};

		float acumX[4];
		float acumY[4];
		return ForEachConvolution(data, width, height, nChannels, 2, 2, 0, 0,
			[&]() {
			Assign<float>(acumX, nChannels, 0.f);
			Assign<float>(acumY, nChannels, 0.f);
		},
			[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				acumX[ii] += src[ii] * sobelX[ic];
				acumY[ii] += src[ii] * sobelY[ic];
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
	RawData* ApplyBox(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY) {

		std::vector<float> box = Normalize({
			1,1,1,
			1,1,1,
			1,1,1,
			});

		float acum[4];
		return ForEachConvolution(data, width, height, nChannels, 2, 2, 0, 0,
			[&]() {
			Assign<float>(acum, nChannels, 0.f);
		},
			[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				acum[ii] += src[ii] * box[ic];
			}
		},
			[&](RawData* dest) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				dest[ii] = clamp(255, 0, acum[ii]);
			}
		});
	}
	RawData* ApplyMedian(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY) {

		float* md[4];
		int sz = 7 * 7;
		int actSz;

		{
			int bsz = sizeof(float) * sz;
			md[0] = (float*)alloca(bsz);
			md[1] = (float*)alloca(bsz);
			md[2] = (float*)alloca(bsz);
			md[3] = (float*)alloca(bsz);
		}

		return ForEachConvolution(data, width, height, nChannels, 7, 7, 3, 3,
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
	RawData* ApplyLaplaceGauss(RawData* data, unsigned int width, unsigned int height, int nChannels,
		unsigned int convWidth, unsigned int convHeight, unsigned int pivotX, unsigned int pivotY) {


		std::vector<float> box = ED::Normalize({
			0,-3,-7,-9,-7,-3,0,
			-3,-16,-23,-18,-23,-16,-3,
			-7,-23,11,65,11,-23,-7,
			-9,-18,65,167,65,-18,-9,
			-7,-23,11,65,11,-23,-7,
			-3,-16,-23,-18,-23,-16,-3,
			0,-3,-7,-9,-7,-3,0
			});

		float acum[4];
		return ForEachConvolution(data, width, height, nChannels, 7, 7, 3, 3,
			[&]() {
			Assign<float>(acum, nChannels, 0.f);
		},
			[&](RawData* src, int ix, int iy, int ic) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				acum[ii] += src[ii] * box[ic];
			}
		},
			[&](RawData* dest) {
			for (int ii = 0; ii < nChannels; ii++)
			{
				dest[ii] = clamp(255, 0, acum[ii]);
			}
		});
	}

	std::vector<float> GetHistogram(RawData* data, unsigned int byteSize, int strideForNextColor, int offsetFrom0, bool normalize)
	{
		std::vector<float> count(256, 0);

		for (size_t ii = offsetFrom0; ii < byteSize; ii += strideForNextColor)
		{
			count[data[ii]]++;
		}

		for (float& c : count)
		{
			c /= (byteSize/strideForNextColor);
		}


		return count;
	}

	int NumberOfUniqueColors(RawData* data, unsigned int byteSize, int strideForNextColor)
	{
		std::set<unsigned int> colors;


		unsigned int aux = 0;

		for (size_t ii = 0; ii < byteSize; ii += strideForNextColor)
		{
			aux = aux & 0;
			for (size_t jj = 0; jj < strideForNextColor; jj++)
			{
				aux = aux << sizeof(RawData);
				aux = aux | (data[ii + jj] & 0xff);

			}
			colors.insert(aux);
		}

		return colors.size();
	}

	bool Load(const std::string& path, RawData*& data, int& width, int& height, int& nChannels)
	{
		std::cout << "loading " << path << "\n";
		data = stbi_load(path.data(), &width, &height, &nChannels, 0);
		return data;
	}

	bool Save(const std::string& fileName, RawData* data, int width, int height, int nChannels)
	{
		return stbi_write_png(fileName.data(), width, height, nChannels, data, nChannels * width);
	}

	bool EDInit()
	{

		EDInitGPU();
		
		//std::cout << "sobel shader" << std::endl;

		//sobelShader.reset(Shader::FromString(vert.c_str(), sobel.c_str()));
		return true;

		/*std::unique_ptr<EDConvolution> boxFilter{ EDConvolution::CreateCustom(
			EDConvolution::Normalize({
				1,1,1,
				1,1,1,
				1,1,1,
				}),
			{ 0,0,0,0 }, 3, 3, 1, 1) };

		std::unique_ptr<EDConvolution> laplacian{ EDConvolution::CreateCustom(
			{
				-1,-1,-1,
				-1,8,-1,
				-1,-1,-1,
			},
			{ 0,0,0,0 }, 3, 3, 1, 1) };*/



		////std::string laplacianConv = BuildShaderConv(
		////	BuildConvolution(laplacian->data, "conv")+
		////	"vec3 avg= vec3(0);\n",
		////	"vec3 color = texture(tex, nUv).rgb;\n"
		////	+ ApplyGreyScale("color") + 
		////	"avg += color * conv[convI];\n",
		////	"fragColor = avg;",
		////	3, 3);

		/*std::string box = BuildShaderConv(
			BuildConvolution(boxFilter->data, "conv") +
			"vec3 avg = vec3(0);\n",

			"vec3 color = texture(tex, nUv).rgb;"
			"avg +=  color * conv[convI];\n",

			"fragColor = vec4(avg, 1);",
			3, 3, 1, 1);*/

		//std::string fragGreyscale = BuildGlobalShader(
		//	ApplyGreyScale("fragColor.rgb")
		//);



		//std::string sobel = BuildShaderConv(
		//	"float convY[9] = float[](\n"
		//	"-1.f, -1.f, -1.f,\n"
		//	"0.f, 0.f, 0.f,\n"
		//	"1.f, 1.f, 1.f\n"
		//	");\n"
		//	"float convX[9] = float[](\n"
		//	"-1.f, 0.f, 1.f,\n"
		//	"-1.f, 0.f, 1.f,\n"
		//	"-1.f, 0.f, 1.f\n"
		//	");\n"
		//	"vec3 avgX = vec3(0);\n"
		//	"vec3 avgY = vec3(0);\n",
		//	"avgX += texture(tex, nUv).rgb * convX[convI];\n avgY += texture(tex, nUv).rgb * convY[convI];\n",
		//	"avg = sqrt((avgX*avgX) + (avgY*avgY));",
		//	3, 3);


		//bwShader = Shader::FromString(vert.c_str(), sobel.c_str());
		//std::cout << "recompiled" << std::endl;

		//std::unique_ptr<RawData*> negative{ EDNegativeHA(img->data, img->GetWidth(), img->GetHeight()) };
		//texId = GetTexture(negative.get(), img->GetWidth(), img->GetHeight());
	}

}