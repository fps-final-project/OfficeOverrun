#pragma once
#include <queue>
#include <memory>

#include "World.h"
#include "GunRig.h"
#include "Camera.hpp"
#include "InputHandler.hpp"
#include "ActionHandler.hpp"
#include "CollisionDetector.hpp"
#include "Player.h"
#include "Pathfinder.h"
#include "SourceVoice.hpp"

class __declspec(dllexport) GameState
{
public:
	std::unique_ptr<World> m_world;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<InputHandler> m_inputHandler;
	std::unique_ptr<ActionHandler> m_actionHandler;
	std::unique_ptr<CollisionDetector> m_collisionDetector;

	std::shared_ptr<Pathfinder> m_pathfinder;

	GameState(
		std::shared_ptr<DirectX::Keyboard> keyboard, 
		std::shared_ptr<DirectX::Mouse> mouse, 
		std::shared_ptr<DX::DeviceResources> deviceResources
	);

	void HandleInput();
	void Update(float dt);
	void CreateWindowSizeDependentResources();

	void RestartWithSeed(int seed);

	bool IsPaused() { return m_isPaused; }
	int GetSeed() { return m_seed; }

	bool GameFinished();
	void TogglePaused();
private:
	std::shared_ptr<DirectX::Keyboard> m_keyboard;
	std::shared_ptr<DirectX::Mouse> m_mouse;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::shared_ptr<std::queue<Action>> m_actionQueue;
	SourceVoice m_music;

	void setupActionHandlers();

	const float FOV = 95.0f;

	bool m_isPaused;
	int m_seed;
};

