#include "pch.h"
#include "FirstPersonShooterMain.h"
#include "Common\DirectXHelper.h"
#include "ResourceManager.h"

using namespace FirstPersonShooter;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
FirstPersonShooterMain::FirstPersonShooterMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.

	ResourceManager::Instance.loadAnimatedModel("Assets\\myarms\\myarms.glb", m_deviceResources, { "Assets\\myarms\\Texture.png" });
	ResourceManager::Instance.loadAnimatedModel("Assets\\myarms\\mygun.glb", m_deviceResources, { "Assets\\myarms\\Texture.png" });

	m_sceneRenderer = std::unique_ptr<Base3DRenderer>(new Base3DRenderer(m_deviceResources));
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	
	AnimatedEntity hands(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	hands.m_animatedModel = ResourceManager::Instance.getAnimatedModel("myarms");
	hands.m_animator = Animator(&hands.m_animatedModel->m_animations["FP_reload"]);
	this->world.m_entities.push_back(hands);


	AnimatedEntity gun(DirectX::XMFLOAT3(0, 0, 0.4572), DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	gun.m_animatedModel = ResourceManager::Instance.getAnimatedModel("mygun");
	gun.m_animator = Animator(&gun.m_animatedModel->m_animations["GUN_reload"]);
	this->world.m_entities.push_back(gun);
	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
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
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void FirstPersonShooterMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		for (auto& entity : world.m_entities)
		{
			m_drawQueue.push(entity.Update(m_timer.GetElapsedSeconds()));
		}
		m_fpsTextRenderer->Update(m_timer);
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
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	while (!m_drawQueue.empty())
	{
		m_sceneRenderer->Render(m_drawQueue.front());
		m_drawQueue.pop();
	}


	m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void FirstPersonShooterMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void FirstPersonShooterMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
