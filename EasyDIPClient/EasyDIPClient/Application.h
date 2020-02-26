#include "EDClientpch.h"





#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__





class Application
{
	//template <typename TT> using ptr = std::shared_ptr<TT>;
	//ImGui::FileBrowser fileDialog;
	unsigned char* imgData;
	int imgWidth;
	int imgHeight;
	int nChannels;
	bool useGPU = true;
	ED::GlobalConv convsGlobal[3] = {
		//ED::ApplyNegative,
		//ED::ApplyGrey,
		//ED::ApplyBW,
	};

	ED::GlobalConv convsGlobalGPU[3] = {
		//ED::ApplyNegativeHA,
		//ED::ApplyGreyHA,
		//ED::ApplyBWHA,
	};

	ED::LocalConv convsLocal[6] = {
		//ED::ApplySobel,
		//ED::ApplyRoberts,
		//ED::ApplyPrewitt,

		//ED::ApplyBox,
		//ED::ApplyMedian,
		//ED::ApplyLaplaceGauss,
	};

	ED::LocalConv convsLocalGPU[6] = {
		//ED::ApplySobel,
		//ED::ApplyRoberts,
		//ED::ApplyPrewitt,

		//ED::ApplyBoxHA,
		//ED::ApplyMedian,
		//ED::ApplyLaplaceGauss,
	};

	GLFWwindow *window;

	//EDImage *img;
	std::unique_ptr<Shader> basicShader;
	unsigned int texId = 0;
	unsigned int texOGImg = 0;

	int windowWidth;
	int windowHeight;
	int widthConv = 7;
	int heightConv = 7;
	int pivotX = 3;
	int pivotY = 3;
	bool show_demo_window = true;

	int top = 0;
	int bottom = 0;
	int left = 0;
	int right = 0;

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

