
#ifndef __ED_PCH__
#define __ED_PCH__

#include <stb_image_write.h>
#include <stb_image.h>
#include <string>
#include <iostream>
#include <istream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <glad/glad.h>
#include <filesystem>
#include <cctype>
#include <cmath>
#include "Quad.h"
#include "shaders/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Shader

namespace ED {
	
#define PARAMS_CONV RawData* imgData, int imgWidth, int imgHeight, int nChannels



	using Padding = std::tuple<int, int, int, int>;
	using Pivot = std::tuple<int, int>;
	using Dimensions = std::tuple<int, int>;
	using RawData = unsigned char;

	struct ConvMeta
	{
		int pivotX;
		int pivotY;
		int pTop, pBot, pLeft, pRight;
		int width, height;
	};






	//using ConvMetaList = std::vector<Kernel>;

	#define PARAMS_GLOBAL RawData* data, unsigned int width, unsigned int height, unsigned int nChannels

	#define PARAMS_LOCAL RawData * data, unsigned int imgWidth, unsigned int imgHeight, int nChannels, \
	std::vector< ConvMeta > meta


	/*!
		@brief We define this as a definition for global convolutions
	*/
	using GlobalConv = std::function<RawData * (PARAMS_GLOBAL)>;

	/*!
		@brief We define this as a definition for local convolutions
	*/
	using LocalConv = std::function<RawData * (PARAMS_LOCAL)>;


	
	template <typename TT> void inline Assign(TT *val, int sz, const TT& eq)
	{
		for (int ii = 0; ii < sz; ii++)
		{
			val[ii] = eq;
		}
	}
	std::string UseForConv(int convWidth, int convHeight, int pivotX, int pivotY, float d, const std::string& op);
	std::string UseForConv(int convWidth, int convHeight, const Pivot &pi, const std::string &op);
	//std::string UseForConv(const Kernel& m, const std::string &op);
	std::string UseForConv(int convWidth, int convHeight, int pivotX, int pivotY, const std::string &op);

	//std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(const std::vector<float> &fullConv, const Kernel m);
	std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(std::vector<float> fullConv, unsigned int actWidth, unsigned int actHeight, int top, int right, int bottom, int left);
	std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(const std::vector<float> &fullConv, unsigned int actWidth, unsigned int actHeight, const Padding &p);

	std::string BuildShaderConv(const std::string& uniforms, const std::string& before, const std::string& op, const std::string& after);

	std::string BuildShaderConv(const std::string& before, const std::string& op, const std::string& after, const std::string& uniforms, int width, int height, int pivotX, int pivotY);
	std::string BuildConvolution(std::vector<float> vals, const std::string& name);
	
	std::string OnlyOnce(const std::string& defName, const std::string& shaderFunc);
	std::string UseGreyScale();
	std::string UseBW();
	std::string UseGradient();

	std::string BuildGlobalShader(const std::string& op, const std::string& uniforms = {});
	
	RawData* ForEachConvolution(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, int convWidth, int convHeight, int pivotX, int pivotY, std::function<void()> init, std::function<void(RawData*, int ix, int iy, int ic)> op, std::function<void(RawData*)> end);
	RawData* ForEachPixel(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, std::function<void(RawData*, RawData*)> op);
	unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight);

	bool EDInit();
}
#endif // !__ED_PCH__



