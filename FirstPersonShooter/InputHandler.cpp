#include "pch.h"
#include "InputHandler.hpp"

void InputHandler::AddEvent(ButtonState state, unsigned int keyCode)
{
	events.emplace(state, keyCode);
}

InputEventArgs InputHandler::PopEvent()
{
	auto res = events.front();
	events.pop();

	return res;
}
