#include "Filter.h"

bool Filter::LoadFromFile(const std::string & path)
{
	std::ifstream file(path);

	while (!file.eof())
	{
		Kernel k;

		std::string pathKernel;
		std::string line;

		std::getline(file, line);
		std::stringstream liness(line);

		liness >> pathKernel;

		// this has to be done first so we override padding and pivot
		k.LoadFromFile(pathKernel);

		liness >> k.pTop;
		liness >> k.pRight;
		liness >> k.pBot;
		liness >> k.pLeft;

		liness >> k.pivotX;
		liness >> k.pivotY;

		kernels.push_back(k);
	}
	return true;
}
