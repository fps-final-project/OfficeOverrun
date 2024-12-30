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
) : m_music(ResourceManager::Instance().getAudioFile("music"), deviceResources->GetXAudio()), m_gameStatus(GameStatus::RUNNING)
{
	m_keyboard = keyboard;
	m_mouse = mouse;
	m_deviceResources = deviceResources;

	m_actionQueue = std::make_shared<std::queue<Action>>();
	m_inputHandler = std::make_unique<InputHandler>(m_actionQueue);
	m_actionHandler = std::make_unique<ActionHandler>(m_actionQueue);

	m_collisionDetector = std::make_unique<SimpleCollisionDetector>();

	this->setupActionHandlers();

	m_music.SetVolume(0.5f);

	RestartWithSeed(123);
}

void GameState::HandleInput()
{
	if (m_gameStatus != GameStatus::RUNNING && m_gameStatus != GameStatus::PAUSED)
		return;

	auto mouseState = m_mouse->GetState();
	auto keyboardState = m_keyboard->GetState();

	if (m_inputHandler->GetEscPressed({ mouseState, keyboardState }))
		TogglePaused();

	m_camera->alignWithMouse(mouseState);
	m_inputHandler->HandleInputState({ mouseState, keyboardState });

	if (m_gameStatus == GameStatus::RUNNING)
		m_actionHandler->HandleActions(m_player.get(), m_world.get(), m_camera.get(), m_deviceResources.get());
}

void GameState::Update(float dt)
{
	if (m_gameStatus != GameStatus::RUNNING)
		return;

	m_player->Update(dt);
	m_world->UpdateCurrentRoom(m_player->getPostition());

	m_pathfinder->UpdatePlayerNode(m_player->getPostition(), m_world->m_currentRoomIndex);

	m_world->UpdateEnemies(m_pathfinder, m_player->getPostition(), m_actionQueue, m_deviceResources);
	m_world->Update(dt);
	m_player->handleRoomCollision(m_world->GetCurrentRoom().checkCollision(m_player->getPostition()));

	m_camera->setPosition(m_player->getPostition());
	m_player->getGunRig()->RotateAndOffset(m_camera->getYawPitchRoll(), m_player->getPostition(), dt);
	m_world->PlayEnemySounds(m_deviceResources, m_player.get());

	std::string gunName;
	if (m_world->IsPlayerNearGun(m_player->getPostition(), gunName))
	{
		m_player->getGunRig()->CollectAmmo(gunName, ResourceManager::Instance().getGunRigMetadata(gunName)->clipSize);
	}

	if (this->GameWon())
	{
		this->ToggleMusicAndMouse();
		m_gameStatus = GameStatus::WON;
	}
	else if (this->GameLost())
	{
		this->ToggleMusicAndMouse();
		m_gameStatus = GameStatus::LOST;
	}

	//TODO: Collision handling

	//auto collisions = m_collisionDetector->GetCollisions(m_world->GetEntities());

}

void GameState::CreateWindowSizeDependentResources()
{
	m_camera->CreateWindowSizeDependentResources(FOV);
}

void GameState::RestartWithSeed(int seed)
{
	while (!m_actionQueue->empty())
	{
		m_actionQueue->pop();
	}

	m_world = std::make_unique<World>();
	m_camera = std::unique_ptr<Camera>(new Camera(m_deviceResources, FOV));
	m_player = std::make_unique<Player>(m_deviceResources->GetXAudio());

	// generating rooms using WorldGenerator
	m_world->m_rooms = MapGeneratorAdapter().WithSeed(seed).GenerateRooms();

	for (int i = 0; i < m_world->m_rooms.size() - 1; i++)
	{
		auto& room = m_world->m_rooms[i];
		room.setModel(
			RoomModelGenerator::generateRoomModel(
				room.getPosition(), room.getSize(), room.getLinks(),
				m_deviceResources));
	}

	PropInstance instance;
	instance.name = "whiteboard";
	instance.position = { 2.f, 0.f, 2.f };
	instance.rotation = { 0.f, 0.f, 0.f };
	instance.size = { 1.64, 1.51, 0.492 };


	//instance.size = { 1.f, 1.f, 1.f };
	m_world->m_rooms[0].m_props.push_back(instance);

	auto& lastRoom = m_world->m_rooms[m_world->m_rooms.size() - 1];
	lastRoom.setModel(
		RoomModelGenerator::generateRoof(
			lastRoom.getPosition(), lastRoom.getSize(), lastRoom.getLinks(),
			m_deviceResources));

	m_pathfinder = std::make_shared<Pathfinder>(m_world->m_rooms, m_player->getPostition());
	m_world->SpawnBaseEnemies(m_pathfinder, m_deviceResources);
	m_world->UpdateVisibleRooms();
	m_world->AddHelicopter();

	m_seed = seed;
	m_gameStatus = GameStatus::RUNNING;
}

bool GameState::GameWon()
{
	return m_world->IsPlayerNearHelicopter(m_player->getPostition());
}

bool GameState::GameLost()
{
	return m_player->isDead();
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

void GameState::TogglePaused()
{
	if (m_gameStatus != GameStatus::PAUSED && m_gameStatus != GameStatus::RUNNING)
		return;

	ToggleMusicAndMouse();
	m_gameStatus = (m_gameStatus == GameStatus::RUNNING) ? GameStatus::PAUSED : GameStatus::RUNNING;
}

void GameState::ToggleMusicAndMouse()
{
	if (m_gameStatus == GameStatus::RUNNING)
	{
		m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
	else
	{
		m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}

	m_music.TogglePlay();
	m_actionHandler->ClearActions();
}
