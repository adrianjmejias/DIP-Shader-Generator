#include "ImGuiFilters.h"

bool ImGuiKernel(ED::Kernel& k)
{
	ImGui::Text("Kernel");

	bool dirty = false;

	if (ImGui::SliderFloat("d", &k.d, -20.f, 20.f))
	{
		dirty = true;
	}
	auto& c = k;
	if (ImGui::InputInt("Convolution Height", &c.width))
	{
		c.width = ED::clamp(7, 1, c.width);
		dirty = true;
	}

	if (ImGui::InputInt("Convolution Width", &c.height))
	{
		c.height = ED::clamp(7, 1, c.height);
		dirty = true;
	}

	if (ImGui::InputInt("Pivot X", &c.pivotX))
	{
		c.pivotX = ED::clamp(c.width-1, 0, c.pivotX);
		dirty = true;
	}

	if (ImGui::InputInt("Pivot Y", &c.pivotY))
	{
		c.pivotY = ED::clamp(c.height - 1, 1, c.pivotY);
		dirty = true;
	}
	if (ImGui::InputInt("padding top", &c.pTop))
	{
		//pivotY = clamp(heightConv - 1, 1, top);
		dirty = true;
	}

	if (ImGui::InputInt("padding right", &c.pRight))
	{
		//pivotY = clamp(heightConv - 1, 1, top);
		dirty = true;
	}

	if (ImGui::InputInt("padding bottom", &c.pBot))
	{
		//pivotY = clamp(heightConv - 1, 1, top);
		dirty = true;
	}

	if (ImGui::InputInt("padding left", &c.pLeft))
	{
		//pivotY = clamp(heightConv - 1, 1, top);
		dirty = true;
	}

	return dirty;
}

bool ImGuiFilter(ED::Filter& filter)
{
	ImGui::Text("Filter");

	bool dirty = false;
	for (int ii=0; ii < filter.kernels.size(); ii++)
	{
		dirty = dirty || ImGuiKernel(filter.kernels[ii]);
	}

	return dirty;
}

bool ImGuiSobel(ED::Sobel& sobel)
{
	bool dirty = false;
	if (ImGui::CollapsingHeader("Sobel Kernel"))
	{
		dirty = dirty || ImGuiFilter(sobel);	
	}
	return dirty;
}
