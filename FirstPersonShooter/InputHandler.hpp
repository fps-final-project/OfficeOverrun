#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include <vector>
#include <functional>

enum class Action
{
	RELOAD,
	SHOOT,
};

typedef std::pair<DirectX::Mouse::State, DirectX::Keyboard::State> InputState;

class InputHandler
{
public:
	InputHandler();
	std::vector<Action> HandleInputState(InputState newState);
	void AddActionHandler(std::function<bool(InputState newState, InputState oldState)> condition, Action action);
	void RemoveActionHandler(Action action);
private:
	std::vector<std::pair<std::function<bool(InputState newState, InputState oldState)>, Action>> m_actions;
	InputState m_inputState;
};

