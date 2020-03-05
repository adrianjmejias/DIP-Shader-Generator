#include "ToonShading.h"


namespace ED
{

RawData* ED::ToonShading::ApplyGPU(PARAMS_CONV)
{

	std::string sobel = BuildGlobalShader(
		"ivec2 ires = textureSize( tex, 0 );\n"
		"float ResS = float( ires.s );\n"
		"float ResT = float( ires.t );\n"
		"vec2 vST = actPos;\n"



		"vec3 irgb = texture( tex, vST ).rgb;\n"
		"vec2 stp0 = vec2(1./ResS, 0. ); // texel offsets\n"
		"vec2 st0p = vec2(0. , 1./ResT);\n"
		"vec2 stpp = vec2(1./ResS, 1./ResT);\n"
		"vec2 stpm = vec2(1./ResS, -1./ResT);\n"
		"// 3x3 pixel colors next\n"
		"vec3 i00 = texture( tex, vST ).rgb;\n"
		"vec3 im1m1 = texture( tex, vST-stpp ).rgb;\n"
		"vec3 ip1p1 = texture( tex, vST+stpp ).rgb;\n"
		"vec3 im1p1 = texture( tex, vST-stpm ).rgb;\n"
		"vec3 ip1m1 = texture( tex, vST+stpm ).rgb;\n"
		"vec3 im10 = texture( tex, vST-stp0 ).rgb;\n"
		"vec3 ip10 = texture( tex, vST+stp0 ).rgb;\n"
		"vec3 i0m1 = texture( tex, vST-st0p ).rgb;\n"
		"vec3 i0p1 = texture( tex, vST+st0p ).rgb;\n"
		"//Single-Image Manipulation 251\n"
		"vec3 target = vec3(0.,0.,0.);\n"
		"target += 1.*(im1m1+ip1m1+ip1p1+im1p1); //apply blur filter\n"
		"target += 2.*(im10+ip10+i0m1+i0p1);\n"
		"target += 4.*(i00);\n"
		"target /= 16.;\n"
		"fragColor = vec4( target, 1. );\n"
		,
		//uniforms
		"uniform mat4 rotMat;\n"
		"uniform float distance;\n" +
		UseGreyScale()
	);


	auto sobelShader(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), sobel.c_str()));
	//sobelShader->use();
	//sobelShader->setMat4("rotMat", rotMat);
	//sobelShader->setFloat("distance", distance);


	return ApplyConvolutionHA(imgData, imgWidth, imgHeight, nChannels, *sobelShader);
}
}
