#pragma once
#include "RoomLabel.h"

namespace WorldGenerator
{
	class EnemyCountSelector
	{
	private:
		static int DefaultRoom();
		static int BossRoom();
	public:
		static int SelectEnemyCount(RoomLabel roomType);
	};
}

