#pragma once
#include "GunProp.h"
#include "Graph.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	class GunPropSelector
	{
	public:
		static std::vector<GunProp> SelectGunsForRoom(const Node<GeneratedRoom>& node);
	private:
		static std::vector<GunProp> GunsInTreasureRoom(const Node<GeneratedRoom>& node);
		static std::vector<GunProp> GunsInDefaultRoom(const Node<GeneratedRoom>& node);
	};
}

