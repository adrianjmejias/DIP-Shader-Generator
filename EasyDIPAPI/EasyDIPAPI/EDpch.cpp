#include "EDpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace ED
{

	
	const std::string SHADER_DEFINE_BW = "DEF_SU_BW";
	const std::string SHADER_DEFINE_GREY = "DEF_SU_GREYSCALE";
	const std::string SHADER_DEFINE_GRADIENT = "DEF_SU_GRADIENT";
		
	float clamp(const float u, const float d, const float v)
	{
		return std::min(std::max(d,v), u);
	}

	std::vector<float> Normalize(std::vector<float> d)
	{
		float sum = 0;

		for (float val : d)
		{
			sum += val;
		}

		for (float& val : d)
		{
			val /= sum;
		}

		return d;
	}

	std::string OnlyOnce(const std::string& defName, const std::string& shaderFunc)
	{
		return
			"//#ifndef "+ defName +"\n"
			"//#define "+ defName + "\n"+
			shaderFunc+
			"//#endif // end" + defName +"\n";
	}
	std::string UseGradient()
	{
		return OnlyOnce(SHADER_DEFINE_GRADIENT,
			"vec3 SU_GRADIENT(vec3 a, vec3 b)\n"
			"{\n"
			"	return sqrt(a*a + b*b);\n"
			"}\n"
		);
	}
	std::string UseGreyScale() {
		return OnlyOnce(SHADER_DEFINE_GREY,
			"vec3 SU_GREYSCALE(vec3 color)"
			"{"
				"float gcolor= dot(color, vec3(0.2125f, 0.7154f, 0.0721f));\n"
				"return vec3(gcolor);\n"
			"}"
		);
	}	  
	std::string UseBW() {
		return OnlyOnce(SHADER_DEFINE_BW,
			UseGreyScale() + 
			"vec3 SU_BW(vec3 color, float threshold)"
			"{"
				"float c = SU_GREYSCALE(color).r;"
				"return vec3(c < threshold ? 0.f : 1.f);\n"
			"}"
		);
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
				//"actPos.y = 1 - actPos.y;\n"
				"fragColor = vec4(texture(tex,actPos).rgb,1);\n"
				+op+
			"}\n");


		return init;
	}


	std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(const std::vector<float> &fullConv, const Kernel m)
	{
		return ReduceConvolution(fullConv, m.width, m.height, m.pTop, m.pRight, m.pBot, m.pLeft);
	}

	
	std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(const std::vector<float> &fullConv, unsigned int actWidth, unsigned int actHeight, const Padding &p)
	{
		return ReduceConvolution(fullConv, actWidth, actHeight, std::get<0>(p), std::get<1>(p), std::get<2>(p), std::get<3>(p));
	}
	std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(std::vector<float> fullConv, unsigned int actWidth, unsigned int actHeight, int top, int right, int bottom, int left)
	{
		bottom = actHeight - bottom;
		right = actWidth - right;
		int newWidth = right - left;
		int newHeight = bottom - top;

		if (newHeight <= 0)
		{
			throw "Bad Vertical boundaries";
		}

		if (newWidth <= 0)
		{
			throw "Bad Horizontal boundaries";
		}

		std::vector<float> reducedConv;
		reducedConv.reserve(static_cast<unsigned int >(newWidth * newHeight));

		for (int ii = top; ii < bottom; ii++) {
			for (int jj = left; jj < right; jj++) {
				reducedConv.push_back(fullConv[ii * actWidth + jj]);
			}
		}
		return { reducedConv, newWidth, newHeight };
	}

	std::string UseForConv(const Kernel& m, const std::string &op) {
		return UseForConv(m.width, m.height, m.pivotX, m.pivotY, op);
	}

	std::string UseForConv(int convWidth, int convHeight, const Pivot &pi, const std::string &op) {
	
		return UseForConv(convWidth, convHeight, std::get<0>(pi), std::get<1>(pi), op);
	}

	std::string UseForConv(int convWidth, int convHeight, int pivotX, int pivotY, const std::string &op) {

		return
			"{\n"
				"vec2 initPos = actPos;"
				"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"
				"vec2 uAcum = vec2(0);\n"
				"int convI = 0;\n"
				"int width = " + std::to_string(convWidth) + ";\n"
				"int height = " + std::to_string(convHeight) + ";\n"
				"vec2 pivotDisplacement = vec2(" + std::to_string(pivotX) + ", " + std::to_string(pivotY) + ") * d;\n"
				"actPos -= pivotDisplacement; \n"
				"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
					"\tfor(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
						"vec2 nUv = actPos + uAcum;\n"
						+ op +
					"\t}\n"
				"}\n"
			"}\n";

	}
	std::string BuildShaderConv(const std::string& uniforms, const std::string& before, const std::string& op, const std::string& after)	{
		std::string init(
			"#version 330 core\n"
			"in vec2 fragPos;\n"
			"uniform sampler2D tex;\n"
			"uniform float imgWidth;\n"
			"uniform float imgHeight;\n"
			"out vec4 fragColor;\n"

			+uniforms+

			"void main(){\n"
			"vec2 actPos = (fragPos.xy + 1)/2.f;\n"
			"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"
				+ before +
				op 
				+ after+
			"}\n"
		);


		return init;

	}

	
	//std::string BuildShaderConv(const std::string& before, const std::string& op, const std::string& after, const std::string& uniforms, int width, int height, int pivotX, int pivotY)	{
	//	std::string init(
	//		"#version 330 core\n"
	//		"in vec2 fragPos;\n"
	//		"uniform sampler2D tex;\n"
	//		"uniform float imgWidth;\n"
	//		"uniform float imgHeight;\n"
	//		"out vec4 fragColor;\n"

	//		+uniforms+

	//		"void main(){\n"
	//		"vec2 actPos = (fragPos.xy + 1)/2.f;\n"
	//		"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"
	//		"vec2 uAcum = vec2(0);\n"
	//		"int convI = 0;\n"
	//		"int width = " + std::to_string(width) + ";\n"
	//		"int height = " + std::to_string(height) + ";\n"
	//		"vec2 pivotDisplacement = vec2("+ std::to_string(pivotX)+", "+ std::to_string(pivotY) + ") * d;\n"
	//		"actPos -= pivotDisplacement; \n"
	//		+ before +
	//		"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
	//			"\tfor(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
	//				"vec2 nUv = actPos + uAcum;\n"
	//				+ op +
	//			"\t}\n"
	//		"}\n"
	//		+ after+
	//	"}\n");


	//	return init;

	//}

	//
	std::string BuildShaderConv(const std::string& before, const std::string& op, const std::string& after, const std::string& uniforms, int width, int height, int pivotX, int pivotY)	{
		std::string init(
			"#version 330 core\n"
			"in vec2 fragPos;\n"
			"uniform sampler2D tex;\n"
			"uniform float imgWidth;\n"
			"uniform float imgHeight;\n"
			"out vec4 fragColor;\n"

			+uniforms+

			"void main(){\n"
				"vec2 actPos = (fragPos.xy + 1)/2.f;\n"
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
				"}\n"
				+ after+
			"}\n"
		
		);


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

	RawData* ForEachConvolution(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, int convWidth, int convHeight, int pivotX, int pivotY,std::function<void()> init, std::function<void(RawData*, int ix, int iy, int ic)> op, std::function<void(RawData*)> end)
	{
		unsigned int byteWidth = width * nChannels;
		int imHeight = height;
		RawData *out = new RawData[ height * byteWidth ];

		int pXFinal = - pivotX * nChannels;
		int pYFinal = - pivotY ;

		for (size_t yy = 0, bcy = 0; 
			yy < imHeight
			; yy += 1, bcy += byteWidth) {
			for (size_t xx = 0; xx < byteWidth; xx += nChannels)
			{
				init();
				{
					for (size_t cy = 0, cdx = 0; cy < convHeight; cy++, cdx += convWidth) {
						int ccy = static_cast<int>(yy) + pYFinal+cy;
						if (ccy < 0) continue;
						
						for (size_t cx = 0, cxc = 0; cx < convWidth; cx++, cxc += nChannels)
						{
							int fx = static_cast<int>(xx) + cxc + pXFinal;
							if (fx < 0) continue;
							int fy = ccy + cy;
							op(&data[fx + fy * byteWidth], fx, fy, cdx + cx);
						}
					}
				}
				end(&out[xx + bcy]);
			}
		}
		return out;
	}

	bool Kernel::LoadFromFile(const std::string& path)
	{
		std::ifstream file(path);

		if (file.is_open())
		{
			std::string line;
			std::string token;
			bool normalize;


			std::getline(file, line);
			std::stringstream liness(line);


			liness >> width;
			liness >> height;
			liness >> pivotX;
			liness >> pivotY;

			if (!liness.eof())
			{
				liness >> token;
				normalize = (token.length() == 1) && (std::tolower(token[0]) == 'n');
			}

			float input;
			for (int ii = width * height-1; ii >= 0; ii--)
			{
				if (file.eof())
				{
					throw "malformed kernel";
					__debugbreak();
					return false;
				}
				file >> input;
				values.push_back(input);
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
			
		return true;
	}

}



