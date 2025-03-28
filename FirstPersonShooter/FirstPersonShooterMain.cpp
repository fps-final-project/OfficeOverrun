﻿#include "pch.h"
#include "FirstPersonShooterMain.h"
#include "Common\DirectXHelper.h"
#include <Keyboard.h>
#include <Mouse.h>
#include "ResourceManager.h"
#include "ResourceHelper.hpp"
#include "Skybox.h"
#include "UI.hpp"

using namespace FirstPersonShooter;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
FirstPersonShooterMain::FirstPersonShooterMain(
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<DirectX::Mouse>& mouse,
	const std::shared_ptr<DirectX::Keyboard>& keyboard) :
	m_deviceResources(deviceResources), m_mouse(mouse), m_keyboard(keyboard)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	bool load_only_ak = false;

	ResourceManager::Instance().loadAnimatedModel("Assets\\Enemy\\Zombie\\zombie_war.gltf", m_deviceResources);
	ResourceManager::Instance().loadAnimatedModel("Assets\\Other\\heli\\heli.gltf", m_deviceResources);

	ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\AK\\ak.gltf", m_deviceResources);
	ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\AK\\ak_gun.gltf", m_deviceResources);
	ResourceManager::Instance().loadGunRigMetadata("Assets\\GunRig\\AK\\ak.txt");

	if (!load_only_ak)
	{
		ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\FN\\FN.gltf", m_deviceResources);
		ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\FN\\FN_gun.gltf", m_deviceResources);
		ResourceManager::Instance().loadGunRigMetadata("Assets\\GunRig\\FN\\FN.txt");

		ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\smg\\smg.gltf", m_deviceResources);
		ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\smg\\smg_gun.gltf", m_deviceResources);
		ResourceManager::Instance().loadGunRigMetadata("Assets\\GunRig\\smg\\smg.txt");

		ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\sniper\\sniper.gltf", m_deviceResources);
		ResourceManager::Instance().loadAnimatedModel("Assets\\GunRig\\sniper\\sniper_gun.gltf", m_deviceResources);
		ResourceManager::Instance().loadGunRigMetadata("Assets\\GunRig\\sniper\\sniper.txt");

	}

	ResourceManager::Instance().loadTexture("Assets\\Other\\crosshair\\crosshair.png", m_deviceResources);

	ResourceHelper::createSkyboxMesh("Assets\\Other\\skybox\\skybox.png", m_deviceResources);

	//ResourceManager::Instance().loadModel("Assets\\Other\\bullet\\bullet.obj", m_deviceResources);
	ResourceManager::Instance().loadModel("Assets\\Other\\stairs\\stairs.gltf", m_deviceResources);
	ResourceManager::Instance().loadTexture("Assets\\Other\\wall\\wall.jpg", m_deviceResources);
	ResourceManager::Instance().loadTexture("Assets\\Other\\wall\\floor.jpg", m_deviceResources);
	ResourceManager::Instance().loadTexture("Assets\\Other\\wall\\concrete.jpg", m_deviceResources);

	ResourceManager::Instance().loadTexture("Assets\\Other\\damage\\damage.png", m_deviceResources);
	ResourceManager::Instance().loadTexture("Assets\\empty_texture.png", m_deviceResources, "empty");
	ResourceManager::Instance().loadTexture("Assets\\cube\\ammo.png", m_deviceResources);

	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\dark-horror-background-252905.wav", XAUDIO2_LOOP_INFINITE, m_deviceResources, "music");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\victory.wav", 0, m_deviceResources, "victory");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\ak.wav", 0, m_deviceResources, "ak");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\FN.wav", 0, m_deviceResources, "FN");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\smg.wav", 0, m_deviceResources, "smg");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\sniper.wav", 0, m_deviceResources, "sniper");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\reload.wav", 0, m_deviceResources, "reload");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\zombie.wav", 0, m_deviceResources, "zombie");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\zombie_dying.wav", 0, m_deviceResources, "zombie_dying");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\empty-clip.wav", 0, m_deviceResources, "empty-clip");

	ResourceHelper::LoadAllPropsModels("Assets\\props", m_deviceResources);
	ResourceHelper::LoadAllMuzzleFlashFrames("Assets\\Other\\muzzle_flash", m_deviceResources);


	m_spriteRenderer = std::make_shared<SpriteRenderer>(
		m_deviceResources->GetD3DDeviceContext(),
		ResourceManager::Instance().getTexture("empty"));

	m_fpsTextRenderer = std::shared_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	m_renderMaster = std::make_shared<RenderMaster>(m_deviceResources);

	m_states = std::make_unique<DirectX::CommonStates>(m_deviceResources->GetD3DDevice());
	m_gameState = std::make_unique<GameState>(keyboard, mouse, deviceResources);

	float initialVolume = 0.5;
	m_deviceResources->ChangeVolume(initialVolume);
	m_menu = std::make_shared<Menu>(deviceResources, m_gameState->GetSeed(), initialVolume);

	// HACK TO MAKE SURE CURSOR IS DISABLED BY DEFAULT
	RenderMenu(Windows::Foundation::Size(0, 0));

	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	
	m_gameState->StartMusic();
}

FirstPersonShooterMain::~FirstPersonShooterMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void FirstPersonShooterMain::CreateWindowSizeDependentResources()
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	// useless?
	m_gameState->CreateWindowSizeDependentResources();
}
void FirstPersonShooterMain::Update()
{
	// Update scene objects.
	m_timer.Tick([&]()
		{
			float dt = m_timer.GetElapsedSeconds();


			// do not change this order
			m_gameState->HandleInput();
			m_mouse->EndOfInputFrame();
			m_gameState->Update(dt);
		});

}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool FirstPersonShooterMain::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView* const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);
	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.

	m_renderMaster->setLighting(
		m_gameState->m_player->getGunRig()->GetBarrelOffset(),
		m_gameState->m_player->getGunRig()->IsMuzzleFlashOn(),
		m_gameState->m_camera->getAt());

	m_renderMaster->setupShaders(
		m_gameState->m_camera->getProjectionMatrix(),
		m_gameState->m_camera->getViewMatrix(),
		m_gameState->m_camera->getPosition());

	m_gameState->m_camera->getAt();

	auto queue = m_gameState->m_world->CreateRenderQueue();

	GUID entityToHit = queue.DrawAllAndClear(m_renderMaster);
	m_gameState->m_actionHandler->SetLastHitEntity(entityToHit);

	Skybox::RenderSkybox(m_gameState->m_camera->getPosition(),
		m_renderMaster, ResourceManager::Instance().getModel("skybox"));

	Size outputSize = m_deviceResources->GetOutputSize();

	// render muzzle flash in front of the gun
	m_spriteRenderer->BeginRendering(context, viewport);
	m_gameState->m_player->getGunRig()->RenderMuzzleFlash(m_spriteRenderer, outputSize.Width, outputSize.Height);
	m_spriteRenderer->EndRendering(context);

	// override the depth buffer with a value close to 1, that corresponds to moving everything that got drawn far away from the screen
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 0.999f, 0);
	m_gameState->m_player->Render(m_renderMaster);

	m_spriteRenderer->BeginRendering(context, viewport);
	UI::RenderCrosshair(outputSize, m_spriteRenderer, ResourceManager::Instance().getTexture("crosshair"));
	if (m_gameState->m_world->lastDamage < 4.f)
		UI::RenderDamageIndicator(outputSize, m_spriteRenderer, ResourceManager::Instance().getTexture("damage"));
	UI::RenderHealth(outputSize, m_spriteRenderer, 100, m_gameState->m_player->getHealth());

	auto ammoCapacity = m_gameState->m_player->getAmmoCapacity();
	UI::RenderBulletCapacity(outputSize, m_spriteRenderer, m_fpsTextRenderer, ResourceManager::Instance().getTexture("ammo"), ammoCapacity.first, ammoCapacity.second);
	m_spriteRenderer->EndRendering(context);

	// will not render unless paused
	RenderMenu(outputSize);

	//m_fpsTextRenderer->Render(std::to_string(m_timer.GetFramesPerSecond()), 400, 400, 300, 50);
	//m_fpsTextRenderer->Render(std::to_string(m_gameState->m_player->getPostition().x) + ", " + std::to_string(m_gameState->m_player->getPostition().y) + ", " + std::to_string(m_gameState->m_player->getPostition().z), 400, 400, 300, 50);

	return true;
}

void FirstPersonShooter::FirstPersonShooterMain::RenderMenu(Size outputSize)
{
	m_menu->StartNewFrame();

	switch (m_gameState->GetStatus())
	{
	case GameStatus::PAUSED:
	{
		m_lastMenuResponse = m_menu->RenderDefaultAndGetResponse(outputSize);
		break;
	}
	case GameStatus::WON:
	{
		m_lastMenuResponse = m_menu->RenderGameWonAndGetResponse(outputSize);
		break;
	}
	case GameStatus::LOST:
	{
		m_lastMenuResponse = m_menu->RenderGameLostAndGetResponse(outputSize);
		break;
	}
	default:
		break;
	}

	m_menu->FinishFrame();

	if (m_lastMenuResponse.changeSeedAndRestart)
	{
		m_gameState->ToggleMusicAndMouse();
		m_gameState->RestartWithSeed(m_lastMenuResponse.seed);
		m_lastMenuResponse.changeSeedAndRestart = false;
	}
	
	if (m_lastMenuResponse.volumeChanged)
	{
		m_deviceResources->ChangeVolume(m_lastMenuResponse.volume);
	}

}

bool FirstPersonShooter::FirstPersonShooterMain::ShouldClose()
{
	return m_lastMenuResponse.exit;
}

// Notifies renderers that device resources need to be released.
void FirstPersonShooterMain::OnDeviceLost()
{
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void FirstPersonShooterMain::OnDeviceRestored()
{
	m_fpsTextRenderer->CreateDeviceDependentResources();
	m_gameState->CreateWindowSizeDependentResources();
}
