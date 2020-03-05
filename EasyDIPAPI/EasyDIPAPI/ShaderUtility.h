#pragma once
#include "EDpch.h"

namespace ED
{

	const std::string SHADER_DEFINE_BW = "DEF_SU_BW";
	const std::string SHADER_DEFINE_GREY = "DEF_SU_GREYSCALE";
	const std::string SHADER_DEFINE_GRADIENT = "DEF_SU_GRADIENT";


	RawData clamp(const float u, const float d, const float v);

	/*!
	@brief Takes the float values of the array and converts
			them to 0 to 1 numbers by averaging them values in the array
	*/
	std::vector<float> Normalize(std::vector<float> d);
	void NormalizeNonAlloc(std::vector<float>& d);
	std::string OnlyOnce(const std::string& defName, const std::string& shaderFunc);

	std::string UseGreyScale();

	std::string UseBW();

	std::string BuildGlobalShader(const std::string & op, const std::string & uniforms);

	std::string UseForConv(int convWidth, int convHeight, int pivotX, int pivotY, float d, const std::string & op);
			
	std::string BuildShaderConv(const std::string& uniforms, const std::string& before, const std::string& op, const std::string& after);

	std::string BuildConvolution(std::vector<float> vals, const std::string& name);

	unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight);

	RawData* ForEachPixel(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, std::function<void(RawData*, RawData*)> op);

	RawData* ForEachConvolution(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, int convWidth, int convHeight, int pivotX, int pivotY, std::function<void()> init, std::function<void(RawData*, int ix, int iy, int ic)> op, std::function<void(RawData*)> end);

	RawData* ApplyConvolutionHA(RawData* data, unsigned int imgWidth, unsigned int imgHeight, unsigned int nChannels, Shader& s);
}