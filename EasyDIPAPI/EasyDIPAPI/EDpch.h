
#ifndef __ED_PCH__
#define __ED_PCH__

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <glad/glad.h>
#include <filesystem>

#include "Quad.h"
#include "shaders/Shader.h"
//Shader

namespace ED {


	using RawData = unsigned char;

	

	std::string BuildShaderConv(const std::string& before, const std::string& op, const std::string& after, int width, int height, int pivotX, int pivotY);
	std::string BuildConvolution(std::vector<float> vals, const std::string& name);
	std::string ApplyGreyScale(const std::string& varName);
	std::string BuildGlobalShader(const std::string& op, const std::string& uniforms = {});
	
	RawData* ForEachConvolution(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, int convWidth, int convHeight, int pivotX, int pivotY, std::function<void(RawData*, int ix, int iy)> op, std::function<void(RawData*)> end, std::function<void()> init);
	RawData* ForEachPixel(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, std::function<void(RawData*, RawData*)> op);
	unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight);

	bool EDInit();
}
#endif // !__ED_PCH__



