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
	const std::shared_ptr<DirectX::Mouse>& mouse,
	const std::shared_ptr<DirectX::Keyboard>& keyboard) :
	m_deviceResources(deviceResources), m_mouse(mouse), m_keyboard(keyboard)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.

	ResourceManager::Instance.loadAnimatedModel("Assets\\myarms\\myarms.glb", m_deviceResources, { "Assets\\myarms\\Texture.png" });
	ResourceManager::Instance.loadAnimatedModel("Assets\\myarms\\mygun.glb", m_deviceResources, { "Assets\\myarms\\Texture.png" });

	ResourceManager::Instance.loadModel("Assets\\AK-47\\AK47NoSubdiv_cw.obj", m_deviceResources);
	ResourceManager::Instance.loadTexture("Assets\\cube\\crosshair.png", m_deviceResources);

	ResourceManager::Instance.loadModel("Assets\\bullet\\bullet.obj", m_deviceResources);

	m_modelRenderer = std::unique_ptr<ModelRenderer>(new ModelRenderer(m_deviceResources));
	m_animatedRenderer = std::unique_ptr<AnimatedModelRenderer>(new AnimatedModelRenderer(m_deviceResources));
	m_spriteRenderer = std::make_unique<SpriteRenderer>(m_deviceResources->GetD3DDeviceContext());
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	m_states = std::make_unique<DirectX::CommonStates>(m_deviceResources->GetD3DDevice());
	m_world = std::unique_ptr<World>(new World());
	m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV));
	m_inputHandler = std::make_unique<InputHandler>();
	m_collisionDetector = std::make_unique<SimpleCollisionDetector>();

	m_gunRig = std::make_unique<GunRig>(
		ResourceManager::Instance.getAnimatedModel("myarms"),
		ResourceManager::Instance.getAnimatedModel("mygun"),
		XMFLOAT3(0.f, 0.f, 0.4572f),
		XMFLOAT3(-0.118846, -0.106299, 0.55291));

	m_world->m_entities.push_back(Entity(ResourceManager::Instance.getModel("AK47NoSubdiv_cw"), XMFLOAT3(5.f, -1.f, 5.f)));

	//m_world->m_entities.push_back(Entity(ResourceManager::Instance.getModel("AK47NoSubdiv_cw"), XMFLOAT3(0.f, -1.f, 5.f)));

	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.first.leftButton && !oldState.first.leftButton; },
		Action::SHOOT
	);
	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.R && oldState.second.R; },
		Action::RELOAD
	);


	//TODO: Change the timer settings if you want something other than the default variable timestep mode.
	//e.g. for 60 FPS fixed timestep update logic, call:

   //m_timer.SetFixedTimeStep(true);
   //m_timer.SetTargetElapsedSeconds(1.0 / 10);

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


	// Update scene objects.
	m_timer.Tick([&]()
		{
			auto mouseState = m_mouse->GetState();
			auto keyboardState = m_keyboard->GetState();

			m_camera->alignWithMouse(mouseState);
			m_gunRig->rotate(m_camera->getYawPitchRoll());

			std::vector<Action> actions = m_inputHandler->HandleInputState({ mouseState, keyboardState });

			for (auto& action : actions)
			{
				if (action == Action::SHOOT)
				{
					if (m_gunRig->isIdle())
					{
						m_gunRig->shoot();
						auto vector_at = m_gunRig->calculateBulletDirection(m_camera->getAt());
						XMFLOAT3 v;
						DirectX::XMStoreFloat3(&v, DirectX::XMVectorScale(vector_at, 10.f));

						auto barrelOffset = m_gunRig->getBarrelOffset();
						m_world->m_timedEntities.push_back(std::make_pair(
							Entity(ResourceManager::Instance.getModel("bullet"), barrelOffset,
								{ 1.f, 1.f, 1.f }, GunRig::calculateBulletOrientation(m_camera->getYawPitchRoll()), v),
							3.f
						));
					}
				}

				if (action == Action::RELOAD)
				{
					if (m_gunRig->isIdle())
					{
						m_gunRig->reload();
					}
				}
			}

			float dt = m_timer.GetElapsedSeconds();
			m_gunRig->update(dt);
			m_world->Update(dt);
			m_fpsTextRenderer->Update(m_timer);

			auto collisions = m_collisionDetector->GetCollisions(m_world->GetEntities());
			std::for_each(
				collisions.begin(),
				collisions.end(),
				[this](std::pair<Hittable, Hittable>& pair) { m_world->DeleteEntity(pair.first); m_world->DeleteEntity(pair.second); }
			);

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
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.

	// ANIMATED ENTITIES
	m_animatedRenderer->setProjectionMatrix(m_camera->getProjectionMatrix());
	m_animatedRenderer->setViewMatrix(m_camera->getViewMatrix());
	m_animatedRenderer->setCameraPosition(m_camera->getPosition());

	m_animatedRenderer->use();
	for (const auto& entity : m_world->m_animatedEntities)
	{
		m_animatedRenderer->Render(entity);
	}

	for (const auto& entity : m_gunRig->getEntites())
	{
		m_animatedRenderer->Render(*entity);
	}
	// REGULAR ENTITES
	m_modelRenderer->setProjectionMatrix(m_camera->getProjectionMatrix());
	m_modelRenderer->setViewMatrix(m_camera->getViewMatrix());
	m_modelRenderer->setCameraPosition(m_camera->getPosition());

	m_modelRenderer->use();
	for (const auto& entity : m_world->m_entities)
	{
		m_modelRenderer->Render(entity);
	}
	for (const auto& entity : m_world->m_timedEntities)
	{
		m_modelRenderer->Render(entity.first);
	}

	m_spriteRenderer->BeginRendering(context, viewport);
	Size outputSize = m_deviceResources->GetOutputSize();
	int size = 100;
	m_spriteRenderer->Render(ResourceManager::Instance.getTexture("crosshair"),
		(outputSize.Width - size) / 2,
		(outputSize.Height - size) / 2,
		size,
		size);
	m_spriteRenderer->EndRendering(context);

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
