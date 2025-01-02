#include "pch.h"
#include "GunPropSelector.h"
#include "RNG.h"
#include "RoomContentConfig.h"
#include "GunPropFactory.h"
#include "RoomLabel.h"

using namespace WorldGenerator;

std::vector<GunProp> GunPropSelector::SelectGunsForRoom(const Node<GeneratedRoom>& node)
{
    switch (node.label)
    {
    case RoomLabel::Treasure:
        return GunsInTreasureRoom();
    default:
        return GunsInDefaultRoom();
    }
}

std::vector<GunProp> WorldGenerator::GunPropSelector::GunsInTreasureRoom()
{
    std::vector<GunProp> props;

    // Add snipers and AKs
    int ak_sniper_count = RNG::RandIntInRange(RoomContentConfig::MIN_AK_SNIPER_IN_TREASURE_ROOM, RoomContentConfig::MAX_AK_SNIPER_IN_TREASURE_ROOM);
    for (int i = 0; i < ak_sniper_count; i++)
    {
        bool add_sniper = RNG::RandBoolWithProbabilty(RoomContentConfig::SNIPER_PROBABILITY);
        if (add_sniper)
            props.push_back(*GunPropFactory::CreateGunProp(Sniper));
        else
            props.push_back(*GunPropFactory::CreateGunProp(AK));
    }

    return props;
}

std::vector<GunProp> WorldGenerator::GunPropSelector::GunsInDefaultRoom()
{
    return std::vector<GunProp>();
}
