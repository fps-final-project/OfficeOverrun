#pragma once

#include <queue>
#include "InputEvent.hpp"

class InputHandler
{
public:
	void AddEvent(ButtonState state, unsigned int code);
	InputEventArgs PopEvent();

private:
	std::queue<InputEventArgs> events;
};

