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

    io.Fonts->AddFontFromFileTTF("Assets\\fonts\\arial.ttf", 56.0f);
}

Menu::~Menu()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplUwp_Shutdown();
    ImGui::DestroyContext();

}

void Menu::Render(Windows::Foundation::Size screenSize)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplUwp_NewFrame();
    ImGui::NewFrame();

    auto size = m_deviceResources->GetLogicalSize();

    if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        const ImVec2 windowSize = ImVec2(300, 200);

        // Calculate the centered position
        ImVec2 windowPos = ImVec2(
            (screenSize.Width - windowSize.x) * 0.5f,
            (screenSize.Height - windowSize.y) * 0.5f
        );

        ImGui::SetWindowPos(windowPos, ImGuiCond_Always);
        ImGui::SetWindowSize(windowSize, ImGuiCond_Always);

        // Center the button within the window
        ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("Exit").x - ImGui::GetStyle().FramePadding.x * 2) * 0.5f);
        ImGui::SetCursorPosY((windowSize.y - ImGui::GetFrameHeight()) * 0.5f);

        // Create the "Exit" button
        if (ImGui::Button("Exit"))
        {
            // Handle exit logic here
            // For example: Close the application or return to a previous menu
            ImGui::End(); // Properly close the current ImGui window
            exit(0);      // Replace this with your application's exit logic
        }
    }

    ImGui::End();

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
