

#ifndef __ED_IMAGE__
#define __ED_IMAGE__

#include "EDpch.h"


float clamp(const float u, const float d, const float v);
#include <stb_image_write.h>
#include <stb_image.h>

class EDImage
{
public:
	static bool TryLoad(const std::string& path, EDImage*& img);

	static bool TrySave(const EDImage& img, const std::string& fileName);
	static bool TrySave(const ED::RawData* data, const std::string fileName, int width, int height, int nChannels) {
		return stbi_write_png(fileName.data(), width, height, nChannels, data, nChannels * width);
	}


	static EDImage* CreateCanvas(const EDImage& other, EDImage* destiny = nullptr);
	
	unsigned char* Get(int x, int y) const;
	void Set(int x, int y, unsigned char* color, int nChannels);
	~EDImage();
	int GetWidth() const;
	int GetHeight() const;
	int GetNChannels() const;
	int GetWidthChannels() const;

	static bool Compatibles(const EDImage& imA, const EDImage& imB);
	static EDImage* Operate(const EDImage& imA, const EDImage& imB, std::function<void(unsigned char*, unsigned char*, unsigned char*, int)> operation, EDImage* destiny = nullptr);
	static EDImage* Add(const EDImage& imA, const EDImage& imB, EDImage* destiny = nullptr);
	static EDImage* Gradient(const EDImage& imA, const EDImage& imB, EDImage* destiny = nullptr);
	static EDImage* Multiply(const EDImage& imA, const EDImage& imB, EDImage* destiny = nullptr);
	static EDImage* Divide(const EDImage& imA, const EDImage& imB, EDImage* destiny = nullptr);
	static EDImage* Substract(const EDImage& imA, const EDImage& imB, EDImage* destiny = nullptr);
	static void GetHistogram(unsigned char* data, int width, int height, int nChannels) {
		//unsigned char * data = 
	}

	static std::multiset<unsigned char> GetUniqueColors(const EDImage& im, int channel)
	{
		return GetUniqueColors(im.data, im.width, im.height, im.nChannels, channel);
	}

	static std::multiset<unsigned char> GetUniqueColors(unsigned char * data, unsigned int width, unsigned int height, int nChannels, int channel)
	{
		unsigned int sz = width * height * nChannels;
		std::multiset<unsigned char> colors;
		
		for (size_t ii = 0; ii < sz; ii += nChannels)
		{
			colors.insert(data[ii + channel]);
		}

		return colors;
	}

	static unsigned int GetUniqueColors(const EDImage& im)
	{
		return GetUniqueColors(im.data, im.width, im.height, im.nChannels);
	}
	static unsigned int GetUniqueColors(unsigned char * data, unsigned int width, unsigned int height, int nChannels)
	{
		unsigned int sz = width * height * nChannels;
		std::unordered_map<int, int> colors;

		for (size_t ii = 0; ii < sz; ii += nChannels)
		{
			int key = 0;
			for (int channel = 0; channel < nChannels; channel++)
			{
				key |= data[ii + channel];
				key = key << sizeof(char);
			}
			//if ((iterator it = colors.find(key)) != end(colors))
			//{
			//	it->

			//}
			
		}

		return colors.size();
	}

	unsigned char* data;
private:

	unsigned char* defColor = new unsigned char[4] { 0,0,0,0 };
	size_t widthChannels;
	int width, height, nChannels;
	EDImage();
};

#endif // !__ED_IMAGE__

