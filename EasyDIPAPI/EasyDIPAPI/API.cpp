
#include "API.h"

namespace ED {

	std::unique_ptr<Quad> quad;

	std::unique_ptr<Shader> greyShader;
	std::unique_ptr<Shader> bwShader;
	std::unique_ptr<Shader> sobel;
	std::unique_ptr<Shader> roberts;
	std::unique_ptr<Shader> prewitt;
	std::unique_ptr<Shader> box;
	std::unique_ptr<Shader> median;
	std::unique_ptr<Shader> laplaceGauss;

	RawData* ApplyGreyHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels) {
		return ApplyConvolutionHA(data, width, height, nChannels, *greyShader);
	}
	//RawData* ApplyBWHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplySobelHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyRobertsHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyPrewittHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyBoxHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyMedianHA(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyLaplaceGaussHA(RawData* data, unsigned int width, unsigned int height) {}

	//RawData* PreApplyConvolution()
	//{

	//}

	RawData* ApplyConvolutionHA(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, Shader& s, unsigned int *tex)
	{
		//PreApplyConvolution(tex);
		RawData* out;
		unsigned int texture;

		if (tex == nullptr)
		{
			texture = GetTexture(data, width, height);
		}
		else
		{
			if (*tex == 0)
			{
				*tex = GetTexture(data, width, height);
			}

			texture = *tex;
		}
		
		// tratar de hacer que el user pase el out que quiera usar
		out = new RawData[(width * height) * nChannels];

		s.use();
		//glActiveTexture(GL_TEXTURE0);
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

	//RawData* ApplyGrey(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyBW(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplySobel(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyRoberts(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyPrewitt(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyBox(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyMedian(RawData* data, unsigned int width, unsigned int height) {}
	//RawData* ApplyLaplaceGauss(RawData* data, unsigned int width, unsigned int height) {}

	
	bool EDInit()
	{
		quad.reset(new Quad());
		std::string vert = Shader::GetSrcFromFile("bw.vert");

		std::string fragNegative = BuildGlobalShader(
			//"fragColor = 1 - fragColor;"

			"fragColor = vec4(1,0,0,1);"
		);
		greyShader.reset(Shader::FromString(vert.c_str(), fragNegative.c_str()));
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


		//std::unique_ptr<EDConvolution> sobelX{ EDConvolution::CreateCustom(
		//	{
		//		1, 2, 1,
		//		0, 0, 0,
		//		-1, -2, -1,
		//	},
		//	{ 0,0,0,0 }, 3, 3, 1, 1) };

		//std::unique_ptr<EDConvolution> sobelY{ EDConvolution::CreateCustom(
		//	{
		//		-1 , 0, 1,
		//		-2 , 0, 2,
		//		-1 , 0, 1,
		//	},
		//	{ 0,0,0,0 }, 3, 3, 1, 1) };

		//std::string sobel = BuildShaderConv(
		//	BuildConvolution(sobelY->data, "convY") +
		//	BuildConvolution(sobelX->data, "convX") +
		//	BuildConvolution({ 0,0,0,0,0,0,0,0,0,1 }, "disp") +
		//	"vec3 avgX = vec3(0);\n"
		//	"vec3 avgY = vec3(0);\n"
		//	,

		//	"#define GRADIENT(a,b) sqrt(a*a + b*b)\n"

		//	"vec3 color = texture(tex, nUv).rgb;\n"
		//	//+ ApplyGreyScale("color")+
		//	"avgX +=  color * convX[convI];\n"
		//	"avgY += color * convY[convI];\n"
		//	,

		//	"fragColor = vec4(GRADIENT(avgY, avgX),1);\n",
		//	3, 3, 1, 1);

		//std::string miniSobel = BuildShaderConv(
		//	BuildConvolution({ 1,-1 }, "conv") +
		//	"vec3 avg  = vec3(0); \n",

		//	"vec3 color = texture(tex, nUv).rgb;"
		//	+ ApplyGreyScale("color") +
		//	"avg +=  color * conv[convI];\n",

		//	"fragColor = vec4(avg,1);",
		//	2, 1);

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



		//bwShader;
		//sobel;
		//roberts;
		//prewitt;
		//box;
		//median;
		//laplaceGauss;

		// ... <snip> ... more code
	}

}