#include "EDpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
namespace ED
{
	std::string ApplyGreyScale(const std::string& varName) {

		return "{float color_greyscale_ = dot("+ varName +", vec3(0.2125f, 0.7154f, 0.0721f));\n"
			+ varName + "= vec3(color_greyscale_);\n}";
	}
	std::string ApplyNegative(const std::string& varName) {
		return varName + " = 1- " + varName;
	}
	std::string BuildGlobalShader(const std::string& op, const std::string& uniforms)
	{

		std::string init(
			"#version 330 core\n"
			"in vec2 fragPos;\n"
			"uniform sampler2D tex;\n"
			"out vec4 fragColor;\n"
			+uniforms+

			"void main(){\n"

				"vec2 actPos =(fragPos.xy + 1)/2.f;\n"
				"actPos.y = 1 - actPos.y;\n"
				"fragColor = vec4(texture(tex,actPos).rgb,1);\n"
				+op+
			"}\n");


		return init;
	}
	std::string BuildShaderConv(const std::string& before, const std::string& op, const std::string& after, int width, int height, int pivotX, int pivotY)
	{
		std::string init(
			"#version 330 core\n"
			"in vec2 fragPos;\n"
			"uniform sampler2D tex;\n"
			"uniform float imgWidth;\n"
			"uniform float imgHeight;\n"
			"out vec4 fragColor;\n"
			"void main(){\n"
			"vec2 actPos =(fragPos.xy + 1)/2.f;\n"
			"actPos.y = 1 - actPos.y;\n"
			"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"
			"vec2 uAcum = vec2(0);\n"
			"int convI = 0;\n"
			"int width = " + std::to_string(width) + ";\n"
			"int height = " + std::to_string(height) + ";\n"
			"vec2 pivotDisplacement = vec2("+ std::to_string(pivotX)+", "+ std::to_string(pivotY) + ") * d;\n"
			"actPos -= pivotDisplacement; \n"
			+ before +
			"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
				"\tfor(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
					"vec2 nUv = actPos + uAcum;\n"
					+ op +
				"\t}\n"
			"uAcum.x = 0;\n"
			"}\n"
			+ after+
		"}\n");


		return init;

	}
	std::string BuildConvolution(std::vector<float> vals, const std::string& name)
	{
		std::string convolution = "float "+name+"[" + std::to_string(vals.size()) + "] = float[](";

		std::for_each(begin(vals), end(vals), [&convolution](float a) {
			convolution.append(std::to_string(a) + ",");
		});

		if (vals.size() > 0)
		{
			convolution.pop_back();
		}
		convolution.append(");\n");

		return convolution;
	}
	unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight)
	{
		unsigned int tex;

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		return tex;
		//return 0;
	}


	RawData* ForEachPixel(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, std::function<void(RawData*, RawData*)> op)
	{
		RawData* out = new RawData[width * height * nChannels];
		unsigned int end = width * height * nChannels;
		for (unsigned int ii = 0; ii < end; ii += nChannels)
		{
			op(&out[ii], &data[ii]);
		}
		return out;
	}

	RawData* ForEachConvolution(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, int convWidth, int convHeight, int pivotX, int pivotY, std::function<void(RawData*, int ix, int iy)> op, std::function<void(RawData*)> end, std::function<void()> init)
	{
		RawData *out = new RawData[width * height * nChannels];
		unsigned int byteWidth = width * nChannels;
		int imHeight = height;

		for (size_t yy = 0; yy < imHeight; yy += 1)
		for (size_t xx = 0; xx < byteWidth; xx += nChannels)
		{
			//float acum[4] = { 0,0,0,0 };
			//unsigned char acumDiscrete[4];
			init();
			for (size_t cy = 0; cy < convHeight; cy++)
			for (size_t cx = 0, cxc = 0; cx < convWidth; cx++, cxc += nChannels)
			{
				int fx = static_cast<int>(xx) + cxc - pivotX;
				int fy = static_cast<int>(yy) + cy  - pivotY;

				op(data, fx, fy);
				//unsigned char* color = data(fx, fy);

	/*			for (size_t cc = 0; cc < nChannels; cc++)
				{
					acum[cc] += color[cc] * data[cy * width + cx];
				}*/
			}

			end(&out[xx + yy * width]);

			//for (size_t cc = 0; cc < nChannels; cc++)
			//{
			//	acumDiscrete[cc] = static_cast<unsigned char>(ED::clamp(255.f, 0.f, acum[cc]));
			//}
			//out[xx + yy * byteWidth](xx, yy, acumDiscrete, nChannels);
		}





		return out;
	}

}



