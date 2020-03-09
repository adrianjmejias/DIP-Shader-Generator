#include "EDClientpch.h"
#include "ImGuiFilters.h"
#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__




class Application
{
	//template <typename TT> using ptr = std::shared_ptr<TT>;
	//ImGui::FileBrowser fileDialog;
	std::unique_ptr<ED::Quad> quad;
	unsigned char* imgData;
	int imgWidth;
	int imgHeight;
	int nChannels;
	bool useGPU = true;
	std::vector<float> rhist, ghist, bhist;

	ED::Sobel sobel;
	ED::Embossing embossing;
	ED::ToonShading toon;
	ED::BlackAndWhite bw;
	ED::Box box;
	ED::Greyscale grey;
	ED::Median median;
	ED::Negative negative;
	ED::Prewitt prewitt;
	ED::Roberts roberts;
	ED::LaplacianOfGaussian log;
	std::unique_ptr<ED::RawData> img;


	//GlobalConvList convsGlobal;
	int nLocalConv = 0;
	int nGlobalConv = 0;
	bool dirty = true;


	glm::vec2 scale{ 1 }, translate{ 0 };
	glm::vec1 rot{0};

	glm::mat4 model;

	void TranslateImage(float x, float y);


	GLFWwindow *window;

	//EDImage *img;
	std::unique_ptr<Shader> basicShader;
	ED::TexId texId;
	unsigned int texOGImg = 0;

	int windowWidth;
	int windowHeight;

	bool show_demo_window = true;

	unsigned int nUniqueColors;

public:
	Application();
	~Application();
	void MainLoop();
	void Render();
	void ImGui();

	static void HelpMarker(const char* desc);
};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
#endif // !1

