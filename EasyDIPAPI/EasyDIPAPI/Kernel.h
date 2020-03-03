#pragma once
#include "EDpch.h"

struct Kernel {
public:
	// pivot
	int pivotX;
	int pivotY;

	// padding
	int pTop = 0;
	int pRight = 0;
	int pBot = 0;
	int pLeft = 0;

	// kernel dims
	int width;
	int height;

	// kernel values
	std::vector<float> values;

	static Kernel LoadFromFile(const std::string& path)
	{
		Kernel k;
		std::ifstream file(path);

		if (file.is_open())
		{
			std::string line;
			std::string token;
			bool normalize;

			std::getline(file, line);
			std::stringstream liness(line);

			liness >> k.width;
			liness >> k.height;
			liness >> k.pivotX;
			liness >> k.pivotY;

			if (!liness.eof())
			{
				liness >> token;
				normalize = (token.length() == 1) && (token[0] == 'n');
			}

			float input;
			for (int ii = k.width * k.height - 1; ii >= 0; ii--)
			{
				if (file.eof())
				{
					throw "malformed kernel";
					__debugbreak();

				}
				file >> input;
				k.values.push_back(input);
			}

			// 
			/*
			7 7 3 3 N
			0, -3, -7, -9, -7, -3, 0,
			-3, -16, -23, -18, -23, -16, -3,
			-7, -23, 11, 65, 11, -23, -7,
			-9, -18, 65, 167, 65, -18, -9,
			-7, -23, 11, 65, 11, -23, -7,
			-3, -16, -23, -18, -23, -16, -3,
			0, -3, -7, -9, -7, -3, 0
			*/



		}
		else {
			std::cout << "couldn't open file \n";
		}

		file.close();

		return k;
	}
	static Kernel ReduceConvolution(std::vector<float> fullConv, unsigned int actWidth, unsigned int actHeight, int top, int pRight, int pBot, int left, int pivotX, int pivotY)
	{
		Kernel k;
		pBot = actHeight - pBot;
		pRight = actWidth - pRight;
		int newWidth = pRight - left;
		int newHeight = pBot - top;

		if (newHeight <= 0)
		{
			throw "Bad Vertical boundaries";
		}

		if (newWidth <= 0)
		{
			throw "Bad Horizontal boundaries";
		}

		std::vector<float> &reducedConv = k.values;
		reducedConv.reserve(static_cast<unsigned int >(newWidth * newHeight));

		for (int ii = top; ii < pBot; ii++) {
			for (int jj = left; jj < pRight; jj++) {
				reducedConv.push_back(fullConv[ii * actWidth + jj]);
			}
		}
		

		k.width = newWidth;
		k.height = newHeight;
		k.pTop = top;
		k.pBot = pBot;
		k.pLeft = left;
		k.pRight = pRight;
		k.pivotX = pivotX;
		k.pivotY = pivotY;

		return k;
	}
	void ApplyPadding()
	{
		pBot = height - pBot;
		pRight = width - pRight;
		width = pRight - pLeft;
		height = pBot - pTop;

		if (height <= 0)
		{
			throw "Bad Vertical boundaries";
		}

		if (width <= 0)
		{
			throw "Bad Horizontal boundaries";
		}

		std::vector<float> &reducedConv = values;
		reducedConv.reserve(static_cast<unsigned int >(width * height));

		for (int ii = pTop; ii < pBot; ii++) {
			for (int jj = pLeft; jj < pRight; jj++) {
				reducedConv.push_back(values[ii * width + jj]);
			}
		}
	}

};