#include "pch.h"
#include "Menu.hpp"
#include "imgui.h"
#include "imgui_impl_uwp.h"
#include "imgui_impl_dx11.h"

Menu::Menu(std::shared_ptr<DX::DeviceResources> deviceResources, int currentSeed) : m_deviceResources(deviceResources), currentSeed(currentSeed)
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

    buttonFont = io.Fonts->AddFontFromFileTTF("Assets\\fonts\\arial.ttf", 32.0f);
    labelFont = io.Fonts->AddFontFromFileTTF("Assets\\fonts\\arial.ttf", 16.0f);
}

Menu::~Menu()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplUwp_Shutdown();
    ImGui::DestroyContext();
}

MenuResponse Menu::RenderAndGetResponse(Windows::Foundation::Size screenSize, bool paused)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplUwp_NewFrame();
    ImGui::NewFrame();

    MenuResponse response;

    if (paused)
    {

        if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
        {
            const ImVec2 windowSize = ImVec2(500, 500); // Adjusted height to accommodate spacing
            const ImVec2 windowPos = ImVec2(
                (screenSize.Width - windowSize.x) * 0.5f,
                (screenSize.Height - windowSize.y) * 0.5f
            );


            ImGui::SetWindowPos(windowPos, ImGuiCond_Always);
            ImGui::SetWindowSize(windowSize, ImGuiCond_Always);

            // Define common element size
            const float elementWidth = windowSize.x * 0.7f;
            const float elementHeight = 50.0f;

            // Calculate initial Y position for centering the column
            const float totalHeight = (elementHeight * 3) + (50.0f * 2); // 3 elements + 2 spacings of 50px
            const float initialCursorY = (windowSize.y - totalHeight) * 0.5f;

            // Input field for "game seed"
            ImGui::SetCursorPosY(initialCursorY);
            ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f); // Center horizontally

            ImGui::SetNextItemWidth(elementWidth); // Set the width of the input field
            ImGui::InputInt("##GameSeed", &currentSeed, 0);
            ImGui::PushFont(labelFont); // Use the smaller font
            ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("Game Seed").x) * 0.5f);
            ImGui::Text("Game Seed");
            ImGui::PopFont();

            // Vertical spacing (50px)
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50.0f);

            // "Change Seed and Restart" button
            ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f);
            if (ImGui::Button("Change Seed and Restart", ImVec2(elementWidth, elementHeight)))
            {
                response.changeSeedAndRestart = true;
            }

            // Vertical spacing (50px)
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50.0f);

            // "Exit" button
            ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f);
            if (ImGui::Button("Exit", ImVec2(elementWidth, elementHeight)))
            {
                response.exit = true;
            }
        }

        ImGui::End();

        ImGui::Render();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }


    ImGui::EndFrame();
    
    response.seed = currentSeed;

    return response;
}
