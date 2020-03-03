#pragma once
#include "Kernel.h"
class Filter
{
public:
	std::vector<Kernel> kernels;
	bool LoadFromFile(const std::string& path);
};