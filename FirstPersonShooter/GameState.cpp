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

	m_world = std::make_unique<World>();
	m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV));
	m_player = std::make_unique<Player>();
	m_collisionDetector = std::make_unique<SimpleCollisionDetector>();

	m_world->m_entities.push_back(Entity(ResourceManager::Instance.getModel("AK47NoSubdiv_cw"), 
		XMFLOAT3(5.f, -1.f, -5.f), XMFLOAT3(0.2f, 0.2f, 0.2f)));

	m_world->m_rooms.push_back(Room(XMFLOAT3(-1.f, -1.f, -2.f), XMFLOAT3(4.f, 4.f, 6.f)));
	m_world->m_currentRoomIndex = 0;

	this->setupActionHandlers();

}

void GameState::HandleInput()
{
	auto mouseState = m_mouse->GetState();
	auto keyboardState = m_keyboard->GetState();

	m_camera->alignWithMouse(mouseState);

	m_inputHandler->HandleInputState({ mouseState, keyboardState });
	m_actionHandler->HandleActions(m_player.get(), m_world.get(), m_camera.get());
}

void GameState::Update(float dt)
{
	m_player->Update(dt);
	m_world->Update(dt);

	m_player->handleRoomCollision(m_world->getCurrentRoom().isInBounds(m_player->getPostition()));

	m_camera->setPosition(m_player->getPostition());
	m_player->getGunRig()->RotateAndOffset(m_camera->getYawPitchRoll(), m_player->getPostition(), dt);


	auto collisions = m_collisionDetector->GetCollisions(m_world->GetEntities());
	int a = 0;
	std::for_each(
		collisions.begin(),
		collisions.end(),
		[this](std::pair<Hittable, Hittable>& pair) { m_world->DeleteEntity(pair.first); m_world->DeleteEntity(pair.second); });

}

void GameState::CreateWindowSizeDependentResources()
{
	m_camera->CreateWindowSizeDependentResources(FOV);
}

void GameState::setupActionHandlers()
{
	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.first.leftButton && !oldState.first.leftButton; },
		Action::SHOOT
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.R && oldState.second.R; },
		Action::RELOAD
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.W && oldState.second.W; },
		Action::WALK_FORWARD
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.S && oldState.second.S; },
		Action::WALK_BACKWARD
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.A && oldState.second.A; },
		Action::WALK_LEFT
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.D && oldState.second.D; },
		Action::WALK_RIGHT
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.Space && oldState.second.Space; },
		Action::JUMP
	);

}
