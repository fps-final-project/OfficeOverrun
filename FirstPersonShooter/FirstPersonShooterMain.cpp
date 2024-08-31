#include "pch.h"
#include "FirstPersonShooterMain.h"
#include "Common\DirectXHelper.h"
#include <Keyboard.h>
#include <Mouse.h>
#include "ResourceManager.h"

using namespace FirstPersonShooter;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
FirstPersonShooterMain::FirstPersonShooterMain(
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<DirectX::Mouse>& mouse) :
	m_deviceResources(deviceResources), m_mouse(mouse)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.

	ResourceManager::Instance.loadAnimatedModel("Assets\\myarms\\myarms.glb", m_deviceResources, { "Assets\\myarms\\Texture.png" });
	ResourceManager::Instance.loadAnimatedModel("Assets\\myarms\\mygun.glb", m_deviceResources, { "Assets\\myarms\\Texture.png" });

	ResourceManager::Instance.loadModel("Assets\\AK-47\\AK47NoSubdiv_cw.obj", m_deviceResources);
	ResourceManager::Instance.loadTexture("Assets\\cube\\crosshair.png", m_deviceResources);

	m_modelRenderer = std::unique_ptr<ModelRenderer>(new ModelRenderer(m_deviceResources));
	m_animatedRenderer = std::unique_ptr<AnimatedModelRenderer>(new AnimatedModelRenderer(m_deviceResources));
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceResources->GetD3DDeviceContext());
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	m_states = std::make_unique<DirectX::CommonStates>(m_deviceResources->GetD3DDevice());
	m_world = std::unique_ptr<World>(new World());
	m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV));
	//m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV,
	//	{2.f, 0.f, 0.f, 0.f},
	//	{-1.f, 0.f, 0.f, 0.f},
	//	{0.f, 1.f, 0.f, 0.f}));

	AnimatedEntity arms(ResourceManager::Instance.getAnimatedModel("myarms"));
	arms.setAnimation("FP_reload");
	m_world->m_animatedEntities.push_back(arms);

	AnimatedEntity gun(ResourceManager::Instance.getAnimatedModel("mygun"), XMFLOAT3(0.f, 0.f, 0.4572f));
	gun.setAnimation("GUN_reload");
	m_world->m_animatedEntities.push_back(gun);

	m_world->m_entities.push_back(Entity(ResourceManager::Instance.getModel("AK47NoSubdiv_cw"), XMFLOAT3(0.f, -1.f, 5.f)));

	m_keyboard = std::make_unique<DirectX::Keyboard>();

	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);


	
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
	// useless?
	m_camera->CreateWindowSizeDependentResources(FOV);
}

// Updates the application state once per frame.
void FirstPersonShooterMain::Update() 
{
	auto mouseState = m_mouse->GetState();

	m_camera->alignWithMouse(mouseState);

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_world->update(m_timer.GetElapsedSeconds());
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
	m_spriteBatch->SetViewport(viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	
	// ANIMATED ENTITIES
	m_animatedRenderer->setProjectionMatrix(m_camera->getProjectionMatrix());
	m_animatedRenderer->setViewMatrix(Camera::m_staticViewMatrix);

	m_animatedRenderer->use();
	for (const auto& entity : m_world->m_animatedEntities)
	{
		m_animatedRenderer->Render(entity);
	}

	// REGULAR ENTITES
	m_modelRenderer->setProjectionMatrix(m_camera->getProjectionMatrix());
	m_modelRenderer->setViewMatrix(m_camera->getViewMatrix());

	m_modelRenderer->use();
	for (const auto& entity : m_world->m_entities)
	{
		m_modelRenderer->Render(entity);
	}

	this->Render2DElements(context);

	m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void FirstPersonShooterMain::OnDeviceLost()
{
	m_modelRenderer->ReleaseDeviceDependentResources();
	m_animatedRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void FirstPersonShooterMain::OnDeviceRestored()
{
	m_modelRenderer->CreateDeviceDependentResources();
	m_animatedRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void FirstPersonShooter::FirstPersonShooterMain::Render2DElements(ID3D11DeviceContext3* context)
{
	ID3D11DepthStencilState* pDepthStencilState = nullptr;
	UINT stencilRef;
	context->OMGetDepthStencilState(&pDepthStencilState, &stencilRef);

	ID3D11BlendState* pBlendState = nullptr;
	FLOAT blendFactor[4];
	UINT sampleMask;
	context->OMGetBlendState(&pBlendState, blendFactor, &sampleMask);

	ID3D11RasterizerState* pRasterizerState = nullptr;
	context->RSGetState(&pRasterizerState);

	m_spriteBatch->Begin();
	Size outputSize = m_deviceResources->GetOutputSize();
	int size = 100;
	RECT rect;
	rect.top = (outputSize.Height - size) / 2;
	rect.left = (outputSize.Width - size) / 2;
	rect.bottom = (outputSize.Height + size) / 2;
	rect.right = (outputSize.Width + size) / 2;

	m_spriteBatch->Draw(ResourceManager::Instance.getTexture("crosshair")->shaderResourceView.Get(), rect);
	m_spriteBatch->End();


	context->OMSetDepthStencilState(pDepthStencilState, stencilRef);
	context->OMSetBlendState(pBlendState, blendFactor, sampleMask);
	context->RSSetState(pRasterizerState);

	if(pDepthStencilState) pDepthStencilState->Release();
	if(pBlendState) pBlendState->Release();
	if(pRasterizerState) pRasterizerState->Release();
}
