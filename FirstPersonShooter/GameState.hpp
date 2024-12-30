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

enum class __declspec(dllexport) GameStatus { RUNNING, PAUSED, WON, LOST, END };

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

	GameStatus GetStatus() const { return m_gameStatus; }
	int GetSeed() const { return m_seed; }
	void TogglePaused();
	void ToggleMusicAndMouse();
	
	inline void StartMusic() { m_music.PlaySound(true); };
private:
	std::shared_ptr<DirectX::Keyboard> m_keyboard;
	std::shared_ptr<DirectX::Mouse> m_mouse;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::shared_ptr<std::queue<Action>> m_actionQueue;
	SourceVoice m_music;

	void setupActionHandlers();
	bool GameWon();
	bool GameLost();

	const float FOV = 95.0f;

	GameStatus m_gameStatus;
	int m_seed;
};

