#include "pch.h"
#include "Menu.hpp"
#include "imgui.h"
#include "imgui_impl_uwp.h"
#include "imgui_impl_dx11.h"

Menu::Menu(std::shared_ptr<DX::DeviceResources> deviceResources, int currentSeed) 
	: m_deviceResources(deviceResources), currentSeed(currentSeed), currentVolume(1.f)
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
	titleFont = io.Fonts->AddFontFromFileTTF("Assets\\fonts\\arial.ttf", 48.0f);
}

Menu::~Menu()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplUwp_Shutdown();
	ImGui::DestroyContext();
}

void Menu::StartNewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplUwp_NewFrame();
	ImGui::NewFrame();
}

void Menu::FinishFrame()
{
	ImGui::EndFrame();
}

MenuResponse Menu::RenderDefaultAndGetResponse(Windows::Foundation::Size screenSize)
{
	MenuResponse response;

	if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	{
		// Define common element size
		const float elementWidth = 350.f;
		const float elementHeight = 50.0f;

		// Calculate initial Y position for centering the column
		const float totalHeight = (elementHeight * 5) + 3 * 30.f + 50.f + 2 * 16.f;

		const ImVec2 windowSize = ImVec2(500, totalHeight + 100.f); // Adjusted height to accommodate spacing
		const ImVec2 windowPos = ImVec2(
			(screenSize.Width - windowSize.x) * 0.5f,
			(screenSize.Height - windowSize.y) * 0.5f
		);

		const float initialCursorY = (windowSize.y - totalHeight) * 0.5f;

		ImGui::SetWindowPos(windowPos, ImGuiCond_Always);
		ImGui::SetWindowSize(windowSize, ImGuiCond_Always);

		// Input field for "game seed"
		ImGui::SetCursorPosY(initialCursorY);
		RenderCommonComponents(response, elementWidth, elementHeight, windowSize);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	response.seed = currentSeed;

	return response;
}

MenuResponse Menu::RenderGameWonAndGetResponse(Windows::Foundation::Size screenSize)
{
	MenuResponse response;

	if (ImGui::Begin("FinishMenu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	{
		// Define common element size
		const float elementWidth = 350.f;
		const float elementHeight = 50.0f;

		// Calculate initial Y position for centering the column
		const float totalHeight = ((elementHeight * 5) + 3 * 30.f + 50.f + 2 * 16.f) + 80.f + 50.0f;

		const ImVec2 windowSize = ImVec2(500, totalHeight + 100.f); // Adjusted height to accommodate spacing
		const ImVec2 windowPos = ImVec2(
			(screenSize.Width - windowSize.x) * 0.5f,
			(screenSize.Height - windowSize.y) * 0.5f
		);

		const float initialCursorY = (windowSize.y - totalHeight) * 0.5f;
		ImGui::SetWindowPos(windowPos, ImGuiCond_Always);
		ImGui::SetWindowSize(windowSize, ImGuiCond_Always);

		// Display congratulatory text
		ImGui::SetCursorPosY(initialCursorY);
		ImGui::PushFont(titleFont); // Use a larger font for the congratulatory text
		ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("Congratulations!").x) * 0.5f);
		ImGui::Text("Congratulations!");
		ImGui::PopFont();


		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

		ImGui::PushFont(buttonFont); // Use a larger font for the congratulatory text
		ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("You have escaped").x) * 0.5f);
		ImGui::Text("You have escaped");
		ImGui::PopFont();

		// Vertical spacing (100px)
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);
		RenderCommonComponents(response, elementWidth, elementHeight, windowSize);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return response;
}

MenuResponse Menu::RenderGameLostAndGetResponse(Windows::Foundation::Size screenSize)
{
	MenuResponse response;

	if (ImGui::Begin("FinishMenu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	{
		// Define common element size
		const float elementWidth = 350.f;
		const float elementHeight = 50.0f;

		// Calculate initial Y position for centering the column
		const float totalHeight = ((elementHeight * 5) + 3 * 30.f + 50.f + 2 * 16.f) + 80.f + 20.0f;

		const ImVec2 windowSize = ImVec2(500, totalHeight + 100.f); // Adjusted height to accommodate spacing
		const ImVec2 windowPos = ImVec2(
			(screenSize.Width - windowSize.x) * 0.5f,
			(screenSize.Height - windowSize.y) * 0.5f
		);

		const float initialCursorY = (windowSize.y - totalHeight) * 0.5f;
		ImGui::SetWindowPos(windowPos, ImGuiCond_Always);
		ImGui::SetWindowSize(windowSize, ImGuiCond_Always);

		// Display congratulatory text
		ImGui::SetCursorPosY(initialCursorY);
		ImGui::PushFont(titleFont); // Use a larger font for the congratulatory text
		ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("You have died!").x) * 0.5f);
		ImGui::Text("You have died!");
		ImGui::PopFont();
		
		// Vertical spacing (100px)
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);
		RenderCommonComponents(response, elementWidth, elementHeight, windowSize);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return response;
}

void Menu::RenderCommonComponents(MenuResponse& response, int elementWidth, int elementHeight, ImVec2 windowSize)
{
	// Slider for "Volume"
	ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f); // Center the slider horizontally
	ImGui::SetNextItemWidth(elementWidth); // Set the width of the slider
	ImGui::SliderFloat("##VolumeSlider", &currentVolume, 0.0f, 1.0f, "%.2f");

	ImGui::PushFont(labelFont); // Use the smaller font for the label
	ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("Volume").x) * 0.5f); // Center the label
	ImGui::Text("Volume");
	ImGui::PopFont();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

	// Input field for "game seed"
	ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f); // Center horizontally
	ImGui::SetNextItemWidth(elementWidth); // Set the width of the input field
	ImGui::InputInt("##GameSeed", &currentSeed, 0);
	ImGui::PushFont(labelFont); // Use the smaller font
	ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize("Game seed").x) * 0.5f);
	ImGui::Text("Game seed");
	ImGui::PopFont();

	// Vertical spacing (50px)
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50.0f);

	// "Change Seed and Restart" button
	ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f);
	if (ImGui::Button("Change seed and restart", ImVec2(elementWidth, elementHeight)))
	{
		response.changeSeedAndRestart = true;
	}

	// Vertical spacing (30px)
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

	// "Change Seed and Restart" button
	ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f);
	if (ImGui::Button("Restart with random seed", ImVec2(elementWidth, elementHeight)))
	{
		response.changeSeedAndRestart = true;
		currentSeed = rand();
	}

	// Vertical spacing (30px)
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

	// "Exit" button
	ImGui::SetCursorPosX((windowSize.x - elementWidth) * 0.5f);
	if (ImGui::Button("Exit", ImVec2(elementWidth, elementHeight)))
	{
		response.exit = true;
	}

	response.seed = currentSeed;
	response.volume = currentVolume;
}
