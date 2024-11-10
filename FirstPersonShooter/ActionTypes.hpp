#pragma once

enum class ActionType
{
	NOACTION,
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
	Action(ActionType type = ActionType::NOACTION, ActionArgs args = {}) : type{ type }, args{ args } {};

	ActionArgs args;
	ActionType type;
};