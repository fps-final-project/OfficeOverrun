#pragma once

enum class ButtonState
{
	RELEASED,
	PRESSED,
	CLICKED
};

struct InputEventArgs
{
	ButtonState state;
	unsigned int keyCode;

	InputEventArgs(ButtonState state, unsigned int keyCode) : state{ state }, keyCode{ keyCode } {}
};