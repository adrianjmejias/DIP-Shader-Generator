#include "EDClientpch.h"


#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__

using LocalConvList = std::vector<ED::LocalConv>;
using GlobalConvList = std::vector<ED::GlobalConv>;




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

	ED::Sobel sobel;
	
	//GlobalConvList convsGlobal;
	int nLocalConv = 0;
	int nGlobalConv = 0;
	bool dirty = true;


	glm::vec2 scale{ 1 }, translate{ 0 };
	glm::vec1 rot{0};

	glm::mat4 model;

	std::vector<std::vector< ED::Padding > > paddings;
	std::vector<std::vector< ED::Pivot > > pivots;


	//std::vector<ED::GlobalConv> convsGlobal = {
	//	ED::ApplyNegative,
	//	//ED::ApplyGrey,
	//	//ED::ApplyBW,
	//};

	//std::vector<ED::ConvMetaList> metaGlobalHA;
	//GlobalConvList convsGlobalGPU = {
	//	ED::ApplyNegativeHA,
	//	//ED::ApplyGreyHA,
	//	//ED::ApplyBWHA,
	//};

	//std::vector<ED::ConvMetaList> metaLocal;
	//LocalConvList convsLocal = {
	//	ED::ApplySobel,
	//	//ED::ApplyRoberts,
	//	//ED::ApplyPrewitt,

	//	//ED::ApplyBox,
	//	//ED::ApplyMedian,
	//	//ED::ApplyLaplaceGauss,
	//};
	//std::vector<ED::ConvMetaList> metaLocalHA;
	//LocalConvList convsLocalGPU = {
	//	ED::ApplySobel,
	//	//ED::ApplyRoberts,
	//	//ED::ApplyPrewitt,

	//	//ED::ApplyBoxHA,
	//	//ED::ApplyMedian,
	//	//ED::ApplyLaplaceGauss,
	//};

	GLFWwindow *window;

	//EDImage *img;
	std::unique_ptr<Shader> basicShader;
	unsigned int texId = 0;
	unsigned int texOGImg = 0;

	int windowWidth;
	int windowHeight;

	bool show_demo_window = true;



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

