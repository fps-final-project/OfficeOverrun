#pragma once
#include <Keyboard.h>
#include <Mouse.h>
#include <vector>
#include <queue>
#include <functional>

#include "ActionTypes.hpp"

typedef std::pair<DirectX::Mouse::State, DirectX::Keyboard::State> InputState;

class InputHandler
{
public:
	InputHandler(std::shared_ptr<std::queue<Action>> actionQueue);
	void HandleInputState(InputState newState);
	void AddActionHandler(std::function<bool(InputState newState, InputState oldState)> condition, Action action);
	void RemoveActionHandler(Action action);
	bool GetEscPressed(InputState newState);
private:
	std::shared_ptr<std::queue<Action>> m_actionQueue;
	std::vector<std::pair<std::function<bool(InputState newState, InputState oldState)>, Action>> m_actions;
	InputState m_inputState;
};

