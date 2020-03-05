#include "API_GPU.h"


namespace ED {
	std::unique_ptr<Quad> quad;

	std::unique_ptr<Shader> greyShader;
	std::unique_ptr<Shader> bwShader;
	std::unique_ptr<Shader> negativeShader;
	std::string vert;

	std::unique_ptr<Shader> sobelShader;
	std::unique_ptr<Shader> roberts;
	std::unique_ptr<Shader> prewitt;
	std::unique_ptr<Shader> box;
	std::unique_ptr<Shader> median;
	std::unique_ptr<Shader> laplaceGauss;

	RawData* ApplySobelHA(PARAMS_LOCAL) {

		//auto [convX, newWidthX, newHeightX] = ReduceConvolution(sobelX, meta[0]);
		//auto [convY, newWidthY, newHeightY] = ReduceConvolution(sobelY, meta[1]);

		//		std::string sobel = BuildShaderConv(
		//			uniforms
		//			UseGradient(),
		//			 before
		//			BuildConvolution(convX, "convX") +
		//			BuildConvolution(convY, "convY") +
		//			"vec3 avgX = vec3(0);\n",
		//			 op
		//			UseForConv(meta[0],
		//					"vec3 color = texture(tex, nUv).rgb;\n"
		//					"avgX +=  color * conv[convI];\n"			
		//				) +
		//			UseForConv(meta[1],
		//				"vec3 color = texture(tex, nUv).rgb;\n"
		//				"avgX +=  color * conv[convI];\n"
		//				),
		//			 after
		//			"fragColor = vec4(avgX, 1);\n"
		//		);


		//		sobelShader.reset(Shader::FromString(vert.c_str(), sobel.c_str()));




				return nullptr;// ApplyConvolutionHA(data, imgWidth, imgHeight, nChannels, *sobelShader);
	}




	bool EDInitGPU()
	{

		quad.reset(new Quad());
		vert = Shader::GetSrcFromFile("bw.vert");

		std::string fragNegative = BuildGlobalShader(
			"fragColor = 1 - fragColor;"
		);

		std::string fragGreyscale = BuildGlobalShader(
			//"fragColor = 1 - fragColor;"
			"fragColor.rgb = SU_GREYSCALE(fragColor.rgb);",
			UseGreyScale()
		);

		std::string fragBW = BuildGlobalShader(
			//"fragColor = 1 - fragColor;"
			"fragColor.rgb = SU_BW(fragColor.rgb, 150);",
			UseBW()
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





		std::cout << "grey shader" << std::endl;
		greyShader.reset(Shader::FromString(vert.c_str(), fragGreyscale.c_str()));

		std::cout << "bw shader" << std::endl;
		bwShader.reset(Shader::FromString(vert.c_str(), fragBW.c_str()));
		std::cout << "negative shader" << std::endl;

		negativeShader.reset(Shader::FromString(vert.c_str(), fragNegative.c_str()));

		return true;
	}
}