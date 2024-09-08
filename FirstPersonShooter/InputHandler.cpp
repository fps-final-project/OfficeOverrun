#include "pch.h"
#include "InputHandler.hpp"

InputHandler::InputHandler() : m_actions{}, m_inputState{}
{
}

std::vector<Action> InputHandler::HandleInputState(InputState newState)
{
	std::vector<Action> actions{};
	std::for_each(
		m_actions.begin(), 
		m_actions.end(), [&actions, &newState, this](std::pair<std::function<bool(InputState, InputState)>, Action> pair) 
		{ if (pair.first(newState, m_inputState)) actions.push_back(pair.second); }
	);
	m_inputState = newState;
	return actions;
}

void InputHandler::AddActionHandler(std::function<bool(InputState newState, InputState oldState)> condition, Action action)
{
	m_actions.push_back(std::make_pair(condition, action));
}

void InputHandler::RemoveActionHandler(Action action)
{
	m_actions.erase(
		std::remove_if(
			m_actions.begin(), 
			m_actions.end(), 
			[action](std::pair<std::function<bool(InputState, InputState)>, Action> pair) 
			{ return pair.second == action; }
		),
		m_actions.end());
}
