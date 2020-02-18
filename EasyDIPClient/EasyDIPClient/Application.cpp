#include "Application.h"




Application::Application() {

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		__debugbreak();
		return;
	}

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	 window = glfwCreateWindow(640, 480, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	 if (window == NULL) {
		__debugbreak();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	bool err = gladLoadGL() == 0;

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		__debugbreak();
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	std::string frag = 
		"#version 330 core\n"
		"in vec2 fragPos;\n"
		"out vec4 fragColor;\n"
		"uniform sampler2D tex;\n"
		"void main(){\n"
			"vec2 actPos =(fragPos.xy + 1)/2.f;\n"
			"actPos.y = 1 - actPos.y;\n"
			"fragColor = vec4(texture(tex,actPos).rgb,1);\n"
		"}\n";

	basicShader.reset(Shader::FromString(Shader::GetSrcFromFile("bw.vert").c_str(), frag.c_str()));


	ED::EDInit();

	if (EDImage::TryLoad("../valve.png", img))
	{
		std::cout << "img loaded successfully\n";
	}
	else
	{
		std::cout << "img load fail\n";
		return;
	}
}

Application::~Application() {

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	//delete img;
	//delete composite;
	//delete bw;
}

void Application::Save(EDImage* img, const std::string& path)
{
	if (EDImage::TrySave(*img, path))
	{
		std::cout << "success " << path << " save \n";
	}
	else
	{
		std::cout << "failed " << path << " save \n";
	}
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		//CG::GetMouseClick(io.MouseClicked);
		
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui();

		// Rendering
		ImGui::Render();
		Render();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void Application::Render()
{

	ED::Quad quad;
	basicShader->use();
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, texId);
	basicShader->setInt("tex", 0);
	quad.Bind();
	quad.Draw();
}

void Application::ImGui()
{
	ImGui::Begin("Convolution Editor");
	// (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
	const char* items[] = { 
		"Negative", "Grey Scale","Black and White", 
		"Sobel", "Roberts", "Prewitt",
		"Box", "Median", "Laplacian of Gaussian",
		"Custom"
	};

	static int idxConv = 0;
	static const char* item_current = items[0]; // Here our selection is a single pointer stored outside the object.
	
	if (ImGui::BeginCombo("Convolution type", item_current, 0)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (item_current == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				item_current = items[n];
				idxConv = n;

			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			}
		}
		ImGui::EndCombo();
	}

	bool IsGlobal = idxConv <= 2;
	bool IsLocal = !IsGlobal && idxConv != 9;

	if (!IsGlobal)
	{
		if (ImGui::InputInt("Convolution Height", &heightConv))
		{
			heightConv = clamp(7, 1, heightConv);
		}

		if (ImGui::InputInt("Convolution Width", &widthConv))
		{
			widthConv = clamp(7, 1, widthConv);
		}

		if (ImGui::InputInt("Pivot X", &pivotX))
		{
			pivotX = clamp(widthConv-1, 0, pivotX);
		}

		if (ImGui::InputInt("Pivot Y", &pivotY))
		{
			pivotY = clamp(heightConv - 1, 1, pivotY);
		}
	}
	ImGui::Checkbox("Use GPU", &useGPU);

	if (ImGui::Button("Apply convolution"))
	{
		std::unique_ptr<ED::RawData> negative;
		
		if (IsGlobal)
		{
			auto conv = useGPU ? convsGlobalGPU[idxConv] : convsGlobal[idxConv];

			negative.reset(conv(img->data, img->GetWidth(), img->GetHeight(), img->GetNChannels()));

		}
		else if(IsLocal)
		{
			auto conv = useGPU ? convsLocalGPU[idxConv -3] : convsLocal[idxConv-3];

			negative.reset(conv(img->data, img->GetWidth(), img->GetHeight(), img->GetNChannels(), widthConv, heightConv, pivotX, pivotY));
		}
		else
		{
			//negative.reset(ED::ApplyConvolutionHA(img->data, img->GetWidth(), img->GetHeight(), img->GetNChannels()));
		}

		texId = ED::GetTexture(negative.get(), img->GetWidth(), img->GetHeight());
	}




	//if (ImGui::Button("Save Image"))
	//{
	//	ImGui::SameLine();
	//	//ImGui::InputText("imgFile")
	//}

	//if (ImGui::Button("Load Image"))
	//{
	//	fileDialog.Open();
	//}
	//	fileDialog.Display();


	//if (fileDialog.HasSelected())
	//{
	//	std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
	//	
	//	fileDialog.Close();
	//	fileDialog.ClearSelected();
	//}


	ImGui::End();

}

void Application::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
