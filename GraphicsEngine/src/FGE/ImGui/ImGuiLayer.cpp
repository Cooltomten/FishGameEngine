#include "GraphicsEngine.pch.h"
#include "ImGuiLayer.h"

#include "FGE/Core/Application.h"

#include <ImGui/imgui.h>
#include <Imgui/imgui_impl_dx11.h>
#include <Imgui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_win32.cpp>

FGE::ImGuiLayer::ImGuiLayer()
{
}

FGE::ImGuiLayer::~ImGuiLayer()
{
}

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void FGE::ImGuiLayer::OnAttach()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImFontConfig fontCfg;
	fontCfg.FontDataOwnedByAtlas = true;
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto.ttf", 16.0f, &fontCfg);
	
    // Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
		style.WindowRounding = 0.f;
	//}

	style.Colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.258f, 0.258f, 0.258f, 1.000f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);

	style.Colors[ImGuiCol_Border] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);

	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);

	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);

	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.313f, 0.313f, 0.313f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);

	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
	//style.Colors[ImGuiCol_TabTop] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.258f, 0.258f, 0.258f, 1.000f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.258f, 0.258f, 0.258f, 1.000f);

	//style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.4f, 0.67f, 1.000f, 0.781f);
	//style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.137f, 0.137f, 0.137f, 1.000f);

	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4f, 0.67f, 1.000f, 1.000f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style.ChildRounding = 0;
	style.FrameRounding = 0.f;
	style.GrabMinSize = 7.0f;
	style.PopupRounding = 2.0f;
	style.ScrollbarRounding = 12.0f;
	style.ScrollbarSize = 13.0f;
	style.TabBorderSize = 1.0f;
	style.TabRounding = 1.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(Application::Get().GetWindow()->GetHandle());
    ImGui_ImplDX11_Init(Application::Get().GetWindow()->GetDX11().GetDevice().Get(),
        Application::Get().GetWindow()->GetDX11().GetDeviceContext().Get());

}

void FGE::ImGuiLayer::OnDetach()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void FGE::ImGuiLayer::Begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void FGE::ImGuiLayer::End()
{
    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

LRESULT FGE::ImGuiLayer::UpdateEvents(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
		
    return false;
}
