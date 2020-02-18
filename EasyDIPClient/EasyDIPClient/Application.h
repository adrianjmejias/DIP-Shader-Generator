#include "EDClientpch.h"





#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__





class Application
{
	//template <typename TT> using ptr = std::shared_ptr<TT>;
	//ImGui::FileBrowser fileDialog;

	bool useGPU = true;
	ED::GlobalConv convsGlobal[3] = {
		ED::ApplyNegative,
		ED::ApplyGrey,
		ED::ApplyBW,
	};

	ED::GlobalConv convsGlobalGPU[3] = {
		ED::ApplyNegativeHA,
		ED::ApplyGreyHA,
		ED::ApplyBWHA,
	};

	ED::LocalConv convsLocal[6] = {
		ED::ApplySobel,
		ED::ApplyRoberts,
		ED::ApplyPrewitt,

		ED::ApplyBox,
		ED::ApplyMedian,
		ED::ApplyLaplaceGauss,
	};

	ED::LocalConv convsLocalGPU[6] = {
		ED::ApplySobel,
		ED::ApplyRoberts,
		ED::ApplyPrewitt,

		ED::ApplyBox,
		ED::ApplyMedian,
		ED::ApplyLaplaceGauss,
	};

	GLFWwindow *window;

	EDImage *img;
	std::unique_ptr<Shader> basicShader;
	unsigned int texId = 0;
	unsigned int texOGImg = 0;

	int windowWidth;
	int windowHeight;
	int widthConv = 1;
	int heightConv = 1;
	float color[7][7][4];
	int pivotX = 0, pivotY = 0;
	bool show_demo_window = true;

public:
	Application();
	~Application();
	void Save(EDImage* img, const std::string& path);
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

