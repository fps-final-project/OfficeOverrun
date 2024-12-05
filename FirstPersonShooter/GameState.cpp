#include "pch.h"
#include "GameState.hpp"
#include "ResourceManager.h"
#include "ResourceHelper.hpp"
#include "SimpleCollisionDetector.hpp"
#include "EnemyBuilder.hpp"
#include "ObjectBuilder.hpp"
#include "MapGeneratorAdapter.h"
#include "RoomModelGenerator.h"

GameState::GameState(
	std::shared_ptr<DirectX::Keyboard> keyboard,
	std::shared_ptr<DirectX::Mouse> mouse,
	std::shared_ptr<DX::DeviceResources> deviceResources
) : m_music(ResourceManager::Instance().getAudioFile("music"), deviceResources->GetXAudio())
{
	m_keyboard = keyboard;
	m_mouse = mouse;
	m_deviceResources = deviceResources;

	m_actionQueue = std::make_shared<std::queue<Action>>();

	m_inputHandler = std::make_unique<InputHandler>(m_actionQueue);
	m_actionHandler = std::make_unique<ActionHandler>(m_actionQueue);

	m_world = std::make_unique<World>();
	m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV));
	m_player = std::make_unique<Player>(deviceResources->GetXAudio());
	m_collisionDetector = std::make_unique<SimpleCollisionDetector>();

	EnemyBuilder enemyBuilder{};
	ObjectBuilder objectBuilder{};


	// generating rooms using WorldGenerator
	m_world->m_rooms = MapGeneratorAdapter().GenerateRooms();

	for (int i = 0; i < m_world->m_rooms.size() - 1; i++)
	{
		auto& room = m_world->m_rooms[i];
		room.setModel(
			RoomModelGenerator::generateRoomModel(
				room.getPosition(), room.getSize(), room.getLinks(),
				m_deviceResources));
	}

	auto& lastRoom = m_world->m_rooms[m_world->m_rooms.size() - 1];
	lastRoom.setModel(
		RoomModelGenerator::generateRoof(
			lastRoom.getPosition(), lastRoom.getSize(), lastRoom.getLinks(),
			m_deviceResources));

	m_pathfinder = std::make_shared<Pathfinder>(m_world->m_rooms, m_player->getPostition());
	m_world->UpdateVisibleRooms();
	m_world->AddHelicopter();


	/*auto zombie = enemyBuilder
		.WithNewEnemy(
			ResourceManager::Instance().getAnimatedModel("zombie_war"),
			ResourceManager::Instance().getAudioFile("zombie"),
			deviceResources->GetXAudio()
		)
		.WithHealth(100)
		.WithDamage(10)
		.WithSpeed(0.05f)
		.WithPosition({ 20.f, 0.f, 4.f })
		.WithRotation({ 0.f, 0.f, 0.f })
		.WithVelocity({ 0.f, 0.f, 0.f })
		.WithSize({ 0.8f, 0.8f, 0.8f })
		.WithAttackRadius(0.7f)
		.WithFallbackAnimation("run")
		.WithPath(m_pathfinder)
		.Build();

	m_world->AddEnemy(zombie);*/

	this->setupActionHandlers();

	m_music.PlaySound(true);
}

void GameState::HandleInput()
{
	auto mouseState = m_mouse->GetState();
	auto keyboardState = m_keyboard->GetState();

	m_camera->alignWithMouse(mouseState);

	m_inputHandler->HandleInputState({ mouseState, keyboardState });
}

void GameState::Update(float dt)
{
	m_player->Update(dt);
	m_world->UpdateCurrentRoom(m_player->getPostition());

	m_pathfinder->UpdatePlayerNode(m_player->getPostition(), m_world->m_currentRoomIndex);

	m_world->UpdateEnemies(m_pathfinder, m_player->getPostition(), m_actionQueue, m_deviceResources);
	m_world->Update(dt);
	m_player->handleRoomCollision(m_world->GetCurrentRoom().checkCollision(m_player->getPostition()));

	m_camera->setPosition(m_player->getPostition());
	m_player->getGunRig()->RotateAndOffset(m_camera->getYawPitchRoll(), m_player->getPostition(), dt);
	m_world->PlayEnemySounds(m_deviceResources, m_player.get());

	m_actionHandler->HandleActions(m_player.get(), m_world.get(), m_camera.get(), m_deviceResources.get());

	//TODO: Collision handling

	//auto collisions = m_collisionDetector->GetCollisions(m_world->GetEntities());

}

void GameState::CreateWindowSizeDependentResources()
{
	m_camera->CreateWindowSizeDependentResources(FOV);
}


bool GameState::GameFinished()
{
	return m_world->IsPlayerNearHelicopter(m_player->getPostition()) || m_player->isDead();
}

void GameState::setupActionHandlers()
{
	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.first.leftButton; },
		Action(ActionType::SHOOT)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.R && oldState.second.R; },
		Action(ActionType::RELOAD)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.W && oldState.second.W; },
		Action(ActionType::WALK_FORWARD)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.S && oldState.second.S; },
		Action(ActionType::WALK_BACKWARD)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.A && oldState.second.A; },
		Action(ActionType::WALK_LEFT)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.D && oldState.second.D; },
		Action(ActionType::WALK_RIGHT)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.Space && oldState.second.Space; },
		Action(ActionType::JUMP)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.D1 && oldState.second.D1; },
		Action(ActionType::WEAPON1)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.D2 && oldState.second.D2; },
		Action(ActionType::WEAPON2)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.D3 && oldState.second.D3; },
		Action(ActionType::WEAPON3)
	);

	m_inputHandler->AddActionHandler(
		[](InputState newState, InputState oldState) {	return newState.second.D4 && oldState.second.D4; },
		Action(ActionType::WEAPON4)
	);
}