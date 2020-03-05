#include "ShaderUtility.h"


namespace ED
{
	RawData* ApplyConvolutionHA(RawData* data, unsigned int imgWidth, unsigned int imgHeight, unsigned int nChannels, Shader& s)
	{
		// The texture we're going to render to
		GLint m_viewport[4];

		glGetIntegerv(GL_VIEWPORT, m_viewport);

		GLuint renderedTexture = GetTexture(nullptr, imgWidth, imgHeight);

		GLuint FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			__debugbreak();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, imgWidth, imgHeight);
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
									   //PreApplyConvolution(tex);
		RawData* out;
		unsigned int texture;
		texture = GetTexture(data, imgWidth, imgHeight);

		// tratar de hacer que el user pase el out que quiera usar
		out = new RawData[(imgWidth * imgHeight) * nChannels];

		s.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		s.setInt("tex", 0);
		s.setFloat("imgWidth", static_cast<float>(imgWidth));
		s.setFloat("imgHeight", static_cast<float>(imgHeight));
		Quad quad;
		quad.Bind();
		quad.Draw();

		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, out);
		glDeleteTextures(1, &texture);
		glDeleteTextures(1, &renderedTexture);

		glViewport(0, 0, m_viewport[2], m_viewport[3]);
		return out;
	}

	RawData clamp(const float u, const float d, const float v)
	{
		return std::min(std::max(d, v), u);
	}

	/*!
	@brief Takes the float values of the array and converts
			them to 0 to 1 numbers by averaging them values in the array
	*/
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


	void NormalizeNonAlloc(std::vector<float> &d)
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
	}
	std::string OnlyOnce(const std::string& defName, const std::string& shaderFunc)
	{
		return
			"\n#ifndef " + defName + "\n"
			"#define " + defName + "\n" +
			shaderFunc +
			"\n#endif // end" + defName + "\n";
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
		return 
			UseGreyScale() +
			OnlyOnce(SHADER_DEFINE_BW,
			"vec3 SU_BW(vec3 color, float threshold)"
			"{"
			"float c = SU_GREYSCALE(color).r;"
			"return vec3(c > threshold ? 1.f : 0.f);\n"
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
			+ uniforms +

			"void main(){\n"

			"vec2 actPos =(fragPos.xy + 1)/2.f;\n"
			//"actPos.y = 1 - actPos.y;\n"
			"fragColor = vec4(texture(tex,actPos).rgb,1);\n"
			+ op +
			"}\n");


		return init;
	}

	std::tuple<std::vector<float>, unsigned int, unsigned int> ReduceConvolution(const std::vector<float>& fullConv, unsigned int actWidth, unsigned int actHeight, const Padding& p)
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
		reducedConv.reserve(static_cast<unsigned int>(newWidth * newHeight));

		for (int ii = top; ii < bottom; ii++) {
			for (int jj = left; jj < right; jj++) {
				reducedConv.push_back(fullConv[ii * actWidth + jj]);
			}
		}
		return { reducedConv, newWidth, newHeight };
	}

	//std::string UseForConv(int convWidth, int convHeight, const Pivot& pi, const std::string& op) {

	//	return UseForConv(convWidth, convHeight, std::get<0>(pi), std::get<1>(pi), op);
	//}


	std::string UseForConv(int convWidth, int convHeight, int pivotX, int pivotY, float d, const std::string& op) {

		return
			"\n\n{//----------------\n"
			"vec2 uAcum = vec2(0);\n"
			"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight) * " + std::to_string(d) + ";\n"
			"int convI = 0;\n"
			"int width = " + std::to_string(convWidth) + ";\n"
			"int height = " + std::to_string(convHeight) + ";\n"
			"vec2 pivotDisplacement = vec2(" + std::to_string(pivotX) + ", " + std::to_string(pivotY) + ") * d;\n"
			"vec2 initPos = actPos - pivotDisplacement; \n"
			"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
			"uAcum.x=0;\n"
			"\tfor(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
			"vec2 nUv = initPos + uAcum;\n"
			"// ********************** for operation\n"
			+ op +
			"// ********************** for operation end\n\n\n"
			"\t}\n"
			"}\n"
			"\n\n}//----------------\n";

	}
	std::string UseForConv(int convWidth, int convHeight, int pivotX, int pivotY, const std::string& op) {

		return
			"\n\n{//----------------\n"
			"vec2 uAcum = vec2(0);\n"
			"int convI = 0;\n"
			"int width = " + std::to_string(convWidth) + ";\n"
			"int height = " + std::to_string(convHeight) + ";\n"
			"vec2 pivotDisplacement = vec2(" + std::to_string(pivotX) + ", " + std::to_string(pivotY) + ") * d;\n"
			"vec2 initPos = actPos - pivotDisplacement; \n"
			"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
			"uAcum.x = 0; \n"
			"\tfor(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
			"vec2 nUv = initPos + uAcum;\n"
			"// ********************** for operation\n"
			+ op +
			"// ********************** for operation end\n\n\n"
			"\t}\n"
			"}\n"
			"\n\n}//----------------\n";

	}
	std::string BuildShaderConv(const std::string& uniforms, const std::string& before, const std::string& op, const std::string& after) {
		std::string init(
			"#version 330 core\n"
			"in vec2 fragPos;\n"
			//"uniform float imgHeight;\n"
			//"uniform float imgWidth;\n"
			"uniform sampler2D tex;\n"

			"out vec4 fragColor;\n"


			"// user uniforms ****************************\n"
			+ uniforms +
			"// user uniforms end ****************************\n\n"

			"void main(){\n"
			"ivec2 texsz = textureSize(tex,0);\n"
			"float imgWidth = texsz.x;\n"
			"float imgHeight = texsz.y;\n"
			"vec2 actPos = (fragPos.xy + 1)/2.f;\n"
			"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"
			"// user before ****************************\n"
			+ before +
			"// user before end****************************\n\n\n"
			"// user operation ****************************\n"

			+ op +
			"// user operation end****************************\n\n\n"

			"// user after ****************************\n"

			+ after +
			"// user after end****************************\n\n\n"

			"}\n"
		);

		return init;

	}

	std::string BuildShaderConv(const std::string& uniforms, const std::string& before, const std::string& op, const std::string& after, int width, int height, int pivotX, int pivotY) {
		std::string init(
			"#version 330 core\n"
			"in vec2 fragPos;\n"
			"uniform sampler2D tex;\n"
			"uniform float imgWidth;\n"
			"uniform float imgHeight;\n"
			"out vec4 fragColor;\n"

			+ uniforms +

			"void main(){\n"
			"vec2 actPos = (fragPos.xy + 1)/2.f;\n"
			"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"
			"vec2 uAcum = vec2(0);\n"
			"int convI = 0;\n"
			"int width = " + std::to_string(width) + ";\n"
			"int height = " + std::to_string(height) + ";\n"
			"vec2 pivotDisplacement = vec2(" + std::to_string(pivotX) + ", " + std::to_string(pivotY) + ") * d;\n"
			"actPos -= pivotDisplacement; \n"
			+ before +
			"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
			"\tfor(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
			"vec2 nUv = actPos + uAcum;\n"
			+ op +
			"\t}\n"
			"}\n"
			+ after +
			"}\n"

		);


		return init;

	}

	std::string BuildConvolution(std::vector<float> vals, const std::string& name)
	{
		std::string convolution = "float " + name + "[" + std::to_string(vals.size()) + "] = float[](";

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

	RawData* ForEachConvolution(RawData* data, unsigned int width, unsigned int height, unsigned int nChannels, int convWidth, int convHeight, int pivotX, int pivotY, std::function<void()> init, std::function<void(RawData*, int ix, int iy, int ic)> op, std::function<void(RawData*)> end)
	{
		unsigned int byteWidth = width * nChannels;
		int imHeight = height;
		RawData* out = new RawData[height * byteWidth];

		int pXFinal = -pivotX * nChannels;
		int pYFinal = -pivotY;

		for (size_t yy = 0, bcy = 0; yy < imHeight; yy += 1, bcy += byteWidth) {
			for (size_t xx = 0; xx < byteWidth; xx += nChannels)
			{
				init();
				{
					for (size_t cy = 0, cdx = 0; cy < convHeight; cy++, cdx += convWidth) {
						int ccy = static_cast<int>(yy) + pYFinal + cy;
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
}