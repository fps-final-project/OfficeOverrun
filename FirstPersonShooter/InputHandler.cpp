#include "pch.h"
#include "InputHandler.hpp"

InputHandler::InputHandler(std::shared_ptr<std::queue<Action>> actionQueue) : m_actions{}, m_inputState{}
{
	m_actionQueue = actionQueue;
}

void InputHandler::HandleInputState(InputState newState)
{
	std::for_each(
		m_actions.begin(), 
		m_actions.end(), [&newState, this](std::pair<std::function<bool(InputState, InputState)>, Action> pair) 
		{ if (pair.first(newState, m_inputState)) m_actionQueue->push(pair.second); }
	);
	m_inputState = newState;
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
			{ return pair.second.type == action.type; }
		),
		m_actions.end());
}
