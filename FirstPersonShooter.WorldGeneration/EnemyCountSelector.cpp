#include "pch.h"
#include "EnemyCountSelector.h"
#include "RNG.h"
#include "RoomContentConfig.h"

using namespace WorldGenerator;

int EnemyCountSelector::DefaultRoom()
{
	DistributionParameters params;
	params.poisson_lambda = RoomContentConfig::ENEMY_IN_NORMAL_ROOM_POISSON_LAMBDA;

	return RNG::RandIntInRange(RoomContentConfig::ENEMY_IN_NORMAL_ROOM_MIN, RoomContentConfig::ENEMY_IN_NORMAL_ROOM_MAX, Poisson, params);
}

int EnemyCountSelector::BossRoom()
{
	DistributionParameters params;
	params.poisson_lambda = RoomContentConfig::ENEMY_IN_BOSS_ROOM_POISSON_LAMBDA;

	return RNG::RandIntInRange(RoomContentConfig::ENEMY_IN_BOSS_ROOM_MIN, RoomContentConfig::ENEMY_IN_BOSS_ROOM_MAX, Poisson, params);
}

int EnemyCountSelector::SelectEnemyCount(RoomLabel roomType)
{
	switch (roomType)
	{
	case Normal:
		return DefaultRoom();
	case Stairs:
		return DefaultRoom();
	case Boss:
		return BossRoom();
	default:
		return 0;
	}
}
