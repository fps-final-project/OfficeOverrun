#include "pch.h"
#include "FirstPersonShooterMain.h"
#include "Common\DirectXHelper.h"
#include <Keyboard.h>
#include <Mouse.h>
#include "ResourceManager.h"
#include "ResourceHelper.hpp"
#include "Skybox.h"

using namespace FirstPersonShooter;
using namespace Windows::Foundation;
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

	bool load_only_ak = true;

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


	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\dark-horror-background-252905.wav", XAUDIO2_LOOP_INFINITE, m_deviceResources, "music");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\ak.wav", 0, m_deviceResources, "ak");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\FN.wav", 0, m_deviceResources, "FN");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\smg.wav", 0, m_deviceResources, "smg");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\sniper.wav", 0, m_deviceResources, "sniper");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\reload.wav", 0, m_deviceResources, "reload");
	ResourceManager::Instance().loadAudioFile("Assets\\Audio\\zombie.wav", 0, m_deviceResources, "zombie");


	m_spriteRenderer = std::make_unique<SpriteRenderer>(m_deviceResources->GetD3DDeviceContext());
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	m_renderMaster = std::make_unique<RenderMaster>(m_deviceResources);

	m_states = std::make_unique<DirectX::CommonStates>(m_deviceResources->GetD3DDevice());
	m_gameState = std::make_unique<GameState>(keyboard, mouse, deviceResources);

	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
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

	m_renderMaster->setLighting(m_gameState->m_world->GetLightingData(), m_gameState->m_camera->getAt());
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

	// override the depth buffer with a value close to 1, that corresponds to moving everything that got drawn far away from the screen
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 0.999f, 0);

	m_gameState->m_player->Render(m_renderMaster);


	m_spriteRenderer->BeginRendering(context, viewport);
	Size outputSize = m_deviceResources->GetOutputSize();
	int size = 100;
	m_spriteRenderer->Render(ResourceManager::Instance().getTexture("crosshair"),
		(outputSize.Width - size) / 2,
		(outputSize.Height - size) / 2,
		size,
		size);
	m_spriteRenderer->EndRendering(context);

	auto pos = m_gameState->m_player->getPostition();
	//m_fpsTextRenderer->Render(std::to_string(m_timer.GetFramesPerSecond()));
	m_fpsTextRenderer->Render(std::to_string(pos.x) + ", " +std::to_string(pos.y) + ", " + std::to_string(pos.z));

	return true;
}

bool FirstPersonShooter::FirstPersonShooterMain::ShouldClose()
{
	return m_gameState->GameFinished();
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
