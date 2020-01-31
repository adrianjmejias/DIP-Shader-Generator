
#include "API.h"

namespace ED {

	std::unique_ptr<Quad> quad;

	std::unique_ptr<Shader> greyShader;
	std::unique_ptr<Shader> bwShader;
	std::unique_ptr<Shader> sobelShader;
	std::unique_ptr<Shader> roberts;
	std::unique_ptr<Shader> prewitt;
	std::unique_ptr<Shader> box;
	std::unique_ptr<Shader> median;
	std::unique_ptr<Shader> laplaceGauss;

	RawData* ApplyGreyHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ApplyConvolutionHA(data, width, height, nChannels, *greyShader);
	}
	//RawData* ApplyBWHA(RawData* data, unsigned int width, unsigned int height) {}
	RawData* ApplySobelHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ApplyConvolutionHA(data, width, height, nChannels, *sobelShader);
	}

	//RawData* ApplyRobertsHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyPrewittHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyBoxHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyMedianHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyLaplaceGaussHA(RawData* data, unsigned int width, unsigned int height) {}

	//RawData* PreApplyConvolution()
	//{

	//}

	std::vector<float> Normalize(std::vector<float> d)
	{
		float sum = 0;

		for (float val : d)
		{
			sum += val;
		}

		for (float& val : d)
		{
			val /= sum;
		}

		return d;
	}

	RawData* ApplyConvolutionHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, Shader& s, unsigned int *tex)
	{
		//PreApplyConvolution(tex);
		RawData* out;
		unsigned int texture;
		texture = GetTexture(data, width, height);
		//if (tex == nullptr)
		//{
		//	texture = GetTexture(data, width, height);
		//}
		//else
		//{
		//	if (*tex == 0)
		//	{
		//		*tex = GetTexture(data, width, height);
		//	}

		//	texture = *tex;
		//}
		
		// tratar de hacer que el user pase el out que quiera usar
		out = new RawData[(width * height) * nChannels];

		s.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		s.setInt("tex", 0);
		s.setFloat("imgWidth", width);
		s.setFloat("imgHeight", height);

		quad->Bind();
		quad->Draw();

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, out);
		glDeleteTextures(1, &texture);

		return out;
	}

	RawData* ApplyGrey(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			float c = src[0] * 0.2125f+ src[1] * 0.7154f+ src[2] * 0.0721f;
			dest[0] = c;
			dest[1] = c;
			dest[2] = c;
		});
	}
	RawData* ApplyNegative(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			dest[0] = 1 - src[0];
			dest[1] = 1 - src[1];
			dest[2] = 1 - src[2];
		});
	}
	RawData* ApplyBW(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ForEachPixel(data, width, height, nChannels, [](RawData* dest, RawData* src) {
			float c = src[0] * 0.2125f + src[1] * 0.7154f + src[2] * 0.0721f;
			c = (c > 0.6) ? 1: 0;
			
			dest[0] = c;
			dest[1] = c;
			dest[2] = c;
		});
	}
	
	RawData* ApplySobel(RawData* data, unsigned int width, unsigned int height, int nChannels) {
		std::vector<float> sobelX = {
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1,
		};
		std::vector<float> sobelY = {
			-1 , 0, 1,
			-2 , 0, 2,
			-1 , 0, 1,
		};

		float acumX[4];
		float acumY[4];
		return ForEachConvolution(data, width, height, nChannels, 3, 3, 1, 1,
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
	RawData* ApplyRoberts(RawData* data, unsigned int width, unsigned int height, int nChannels){
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
	RawData* ApplyPrewitt(RawData* data, unsigned int width, unsigned int height, int nChannels) {
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
	RawData* ApplyBox(RawData* data, unsigned int width, unsigned int height, int nChannels) {
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
	RawData* ApplyMedian(RawData* data, unsigned int width, unsigned int height, int nChannels) {
		float* md[4];
		int sz = 7 * 7;
		int actSz;

		{
			int bsz = sizeof(float) * sz;
			md[0] = (float*) alloca(bsz);
			md[1] = (float*) alloca(bsz);
			md[2] = (float*) alloca(bsz);
			md[3] = (float*) alloca(bsz);
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
					dest[ii] = clamp(255, 0, md[ii][actSz/2]);
				}
			}
		);
	
	}
	RawData* ApplyLaplaceGauss(RawData* data, unsigned int width, unsigned int height, int nChannels) {
	
		std::vector<float> box = ED::Normalize({
			0,-3,-7,-9,-7,-3,0,
			-3,-16,-23,-18,-23,-16,-3,
			-7,-23,11,65,11,-23,-7,
			-9,-18,65,167,65,-18,-9,
			-7,-23,11,65,11,-23,-7,
			-3,-16,-23,-18,-23,-16,-3,
			0,-3,-7,-9,-7,-3,0
		});
			//1, 4,7,4,1,
			//4, 16,26,16,4,
			//7, 26,41,26,7,
			//4, 16,26,16,4,
			//1, 4,7,4,1,

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



	
	bool EDInit()
	{
		quad.reset(new Quad());
		std::string vert = Shader::GetSrcFromFile("bw.vert");

		std::string fragNegative = BuildGlobalShader(
			//"fragColor = 1 - fragColor;"
			"fragColor = vec4(1,0,0,1);"
		);

		std::vector<float> sobelX = {
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1,
		};
		std::vector<float> sobelY = {
			-1 , 0, 1,
			-2 , 0, 2,
			-1 , 0, 1,
		};

		std::string sobel = BuildShaderConv(
			BuildConvolution(sobelY, "convY") +
			BuildConvolution(sobelX, "convX") +
			//BuildConvolution({0,0,0,0,0,0,0,0,0,1}, "disp") +
			"vec3 avgX = vec3(0);\n"
			"vec3 avgY = vec3(0);\n"
			,

			"#define GRADIENT(a,b) sqrt(a*a + b*b)\n"
			
			"vec3 color = texture(tex, nUv).rgb;\n"
			"avgX +=  color * convX[convI];\n"
			"avgY += color * convY[convI];\n"
			,

			"fragColor = vec4(GRADIENT(avgY, avgX),1);\n",
			3, 3, 1, 1);


		greyShader.reset(Shader::FromString(vert.c_str(), fragNegative.c_str()));
		sobelShader.reset(Shader::FromString(vert.c_str(), sobel.c_str()));

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