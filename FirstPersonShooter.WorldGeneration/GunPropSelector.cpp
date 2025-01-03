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
        return GunsInTreasureRoom(node);
    default:
        return GunsInDefaultRoom(node);
    }
}

std::vector<GunProp> WorldGenerator::GunPropSelector::GunsInTreasureRoom(const Node<GeneratedRoom>& node)
{
    GeneratedRoom& room = *node.value;
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

    // Add smgs
    DistributionParameters params;
    params.binomial_t = room.size.x * room.size.y * RoomContentConfig::SMGS_IN_TREASURE_ROOM_SIZE_TO_COUNT_COEFF;
    params.binomial_p = RoomContentConfig::SMGS_IN_TREASURE_ROOM_BINOMIAL_P;

    int smg_count = RNG::RandIntInRange(RoomContentConfig::MIN_SMGS_IN_TREASURE_ROOM, RoomContentConfig::MAX_SMGS_IN_TREASURE_ROOM, Binomial, params);

    for (int i = 0; i < smg_count; i++)
        props.push_back(*GunPropFactory::CreateGunProp(Smg));

    return props;
}

std::vector<GunProp> WorldGenerator::GunPropSelector::GunsInDefaultRoom(const Node<GeneratedRoom>& node)
{
    GeneratedRoom& room = *node.value;
    std::vector<GunProp> props;

    DistributionParameters params;
    params.binomial_t = room.size.x * room.size.y * RoomContentConfig::SMGS_IN_DEFAULT_ROOM_SIZE_TO_COUNT_COEFF;
    params.binomial_p = RoomContentConfig::SMGS_IN_DEFAULT_ROOM_BINOMIAL_P;

    int smg_count = RNG::RandIntInRange(RoomContentConfig::MIN_SMGS_IN_DEFAULT_ROOM, RoomContentConfig::MAX_SMGS_IN_DEFAULT_ROOM, Binomial, params);

    for (int i = 0; i < smg_count; i++)
        props.push_back(*GunPropFactory::CreateGunProp(Smg));

    return props;
}
