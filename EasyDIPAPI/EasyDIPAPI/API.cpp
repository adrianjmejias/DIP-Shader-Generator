
#include "API.h"

namespace ED {


	RawData* HistogramToTexture(const std::vector<float> hist, int imgWidth = 256, int imgHeight = 256)
	{
		RawData* data = new RawData[imgWidth * imgHeight];


		for (size_t iW = 0; iW < imgWidth; iW+= 1)
		{
			size_t iH = 0;
			for (size_t end = imgHeight * hist[iW]; iH < end; iH++)
			{
				data[iW + iH * imgWidth] = 255;
			}
			for (; iH < imgHeight; iH++)
			{
				data[iW + iH * imgWidth] = 0;
			}
		}




		return data;
	}

	std::vector<float> GetHistogram(RawData* data, unsigned int byteSize, int strideForNextColor, int offsetFrom0, bool normalize)
	{
		std::vector<float> count(256, 0);

		int idxMax = 0;
		for (size_t ii = offsetFrom0; ii < byteSize; ii += strideForNextColor)
		{
			auto& num = count[data[ii]];
			num++;

			if (num >= count[idxMax])
			{
				idxMax = data[ii];
			}
		}

		float nColors = (byteSize/strideForNextColor);

		
		float valMax = count[idxMax];
		for (float& c : count)
		{
			c /= valMax;
		}


		return count;
	}

	int NumberOfUniqueColors(RawData* data, unsigned int byteSize, int strideForNextColor)
	{
		std::set<unsigned int> colors;

		int aux = 0;

		for (size_t ii = 0; ii < byteSize; ii += strideForNextColor)
		{
			aux = aux & 0;
			for (size_t jj = 0; jj < strideForNextColor; jj++)
			{
				aux = aux << 8;

				int col = data[ii + jj];
				col = col & 0xff;
				aux = aux | col;

			}
			colors.insert(aux);
		}

		return colors.size();
	}

	bool Load(const std::string& path, RawData*& data, int& width, int& height, int& nChannels)
	{
		std::cout << "loading " << path << "\n";
		data = stbi_load(path.data(), &width, &height, &nChannels, 0);
		return data;
	}

	bool Save(const std::string& fileName, RawData* data, int width, int height, int nChannels)
	{
		return stbi_write_png(fileName.data(), width, height, nChannels, data, nChannels * width);
	}


}