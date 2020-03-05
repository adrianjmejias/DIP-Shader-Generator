#pragma once
#include "EDpch.h"


namespace ED
{

struct Kernel {
public:
	float d = 1;	
	bool normalize = false;
	// pivot
	int pivotX;
	int pivotY;

	// padding
	int pTop = 0;
	int pRight = 0;
	int pBot = 0;
	int pLeft = 0;

	// kernel dims
	int width = 0;
	int height= 0;

	// kernel values
	std::vector<float> values;

	int nopad_width;
	int nopad_height;

	std::vector<float> nopad_values;
	static Kernel LoadFromFile(const std::string& path);
	static Kernel ReduceConvolution(std::vector<float> fullConv, unsigned int actWidth, unsigned int actHeight, int top, int pRight, int pBot, int left, int pivotX, int pivotY);
	void ApplyPadding();
};
}