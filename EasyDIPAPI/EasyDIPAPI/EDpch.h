
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
using RawData = unsigned char;



std::unique_ptr<Shader> greyShader;
std::unique_ptr<Shader> bwShader;
std::unique_ptr<Shader> sobel;
std::unique_ptr<Shader> roberts;
std::unique_ptr<Shader> prewitt;
std::unique_ptr<Shader> box;
std::unique_ptr<Shader> median;
std::unique_ptr<Shader> laplaceGauss;

std::string BuildShaderConv(const std::string& before, const std::string& op, const std::string& after, int width, int height, int pivotX, int pivotY);
std::string BuildConvolution(std::vector<float> vals, const std::string& name);
std::string ApplyGreyScale(const std::string& varName);
std::string BuildGlobalShader(const std::string& op, const std::string& uniforms = {});

unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight);

bool EDInit();

#endif // !__ED_PCH__



