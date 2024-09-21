#include "pch.h"
#include "GameState.hpp"
#include "ResourceManager.h"
#include "SimpleCollisionDetector.hpp"

GameState::GameState(
	std::shared_ptr<DirectX::Keyboard> keyboard,
	std::shared_ptr<DirectX::Mouse> mouse,
	std::shared_ptr<DX::DeviceResources> deviceResources
)
{
	m_keyboard = keyboard;
	m_mouse = mouse;
	m_deviceResources = deviceResources;

	m_actionQueue = std::make_shared<std::queue<Action>>();

	m_inputHandler = std::make_unique<InputHandler>(m_actionQueue);
	m_actionHandler = std::make_unique<ActionHandler>(m_actionQueue);

	m_world = std::unique_ptr<World>(new World());
	m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV));
	m_gunRig = std::make_unique<GunRig>(
		ResourceManager::Instance.getAnimatedModel("myarms"),
		ResourceManager::Instance.getAnimatedModel("mygun"),
		XMFLOAT3(0.f, 0.f, 0.4572f),
		XMFLOAT3(-0.118846, -0.106299, 0.55291));
	m_collisionDetector = std::make_unique<SimpleCollisionDetector>();

	m_world->m_entities.push_back(Entity(ResourceManager::Instance.getModel("AK47NoSubdiv_cw"), XMFLOAT3(5.f, -1.f, 5.f)));

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.first.leftButton && !oldState.first.leftButton; },
		Action::SHOOT
	);
	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.R && oldState.second.R; },
		Action::RELOAD
	);

}

void GameState::HandleInput()
{
	auto mouseState = m_mouse->GetState();
	auto keyboardState = m_keyboard->GetState();

	m_camera->alignWithMouse(mouseState);
	m_gunRig->Rotate(m_camera->getYawPitchRoll());

	m_inputHandler->HandleInputState({ mouseState, keyboardState });
	m_actionHandler->HandleActions(m_gunRig.get(), m_world.get(), m_camera.get());
}

void GameState::Update(float dt)
{
	m_gunRig->Update(dt);
	m_world->Update(dt);

	auto collisions = m_collisionDetector->GetCollisions(m_world->GetEntities());
	std::for_each(
		collisions.begin(),
		collisions.end(),
		[this](std::pair<Hittable, Hittable>& pair) { m_world->DeleteEntity(pair.first); m_world->DeleteEntity(pair.second); }
	);
}

void GameState::CreateWindowSizeDependentResources()
{
	m_camera->CreateWindowSizeDependentResources(FOV);
}
