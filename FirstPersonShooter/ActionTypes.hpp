#pragma once

enum class ActionType
{
	RELOAD,
	SHOOT,
	WALK_FORWARD,
	WALK_BACKWARD,
	WALK_LEFT,
	WALK_RIGHT,
	JUMP,
	WEAPON1,
	WEAPON2,
	WEAPON3,
	WEAPON4,
	ATTACK
};

struct AttackArgs
{
	int damage;
};

union ActionArgs 
{
	AttackArgs attack;
};

struct Action 
{
	Action(ActionType type) : type{type} {};

	ActionArgs args;
	ActionType type;
};