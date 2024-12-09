#include "pch.h"
#include "Menu.hpp"
#include "imgui.h"
#include "imgui_impl_uwp.h"
#include "imgui_impl_dx11.h"

Menu::Menu(std::shared_ptr<DX::DeviceResources> deviceResources) : m_deviceResources(deviceResources)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // High DPI scaling
    io.DisplayFramebufferScale = { m_deviceResources->GetDpi() / 96.f, m_deviceResources->GetDpi() / 96.f };                // TODO: Handle DPI change


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplUwp_InitForCurrentView();
    ImGui_ImplDX11_Init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());

}

Menu::~Menu()
{
}

void Menu::Render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplUwp_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
