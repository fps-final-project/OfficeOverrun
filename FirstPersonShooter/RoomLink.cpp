#include "pch.h"
#include "RoomLink.h"
#include "RoomLayoutConfig.h"
#include "RNG.h"

using namespace WorldGenerator;

Orientation WorldGenerator::RoomLink::MakeOrientation(Vector3 size)
{
    if (size.x == 0)
        return Orientation::YZ;
    if (size.y == 0)
        return Orientation::XZ;
    if (size.x < RoomLayoutConfig::verticalRoomLinkLength + 2)
        return Orientation::XYY;
    if (size.y < RoomLayoutConfig::verticalRoomLinkLength + 2)
        return Orientation::XYX;
    return RNG::RandBool() ? Orientation::XYX : Orientation::XYY;
}

RoomLink::RoomLink(const Vector3& pos, const Orientation& orientation)
    : pos(pos), orientation(orientation)
{
}

// TODO: play with rng
RoomLink RoomLink::MakeRoomLink(Vector3 pos, Vector3 size)
{
    Orientation orientation = MakeOrientation(size);

    Vector3 linkPos;
    if (orientation == Orientation::XZ || orientation == Orientation::YZ)
        linkPos = Vector3(
            pos.x + (size.x > 0 ? RNG::RandIntInRange(1, size.x - 2) : 0),
            pos.y + (size.y > 0 ? RNG::RandIntInRange(1, size.y - 2) : 0),
            pos.z
        );
    else
    {
        int xSize = orientation == Orientation::XYX ? RoomLayoutConfig::verticalRoomLinkLength : RoomLayoutConfig::verticalRoomLinkWidth;
        int ySize = orientation == Orientation::XYY ? RoomLayoutConfig::verticalRoomLinkLength : RoomLayoutConfig::verticalRoomLinkWidth;
        linkPos = Vector3(
            pos.x + RNG::RandIntInRange(1, size.x - (xSize + 1)),
            pos.y + RNG::RandIntInRange(1, size.y - (ySize + 1)),
            pos.z - 1
        );
    }
    return RoomLink(linkPos, orientation);
}

bool WorldGenerator::RoomLink::ValidSizeForRoomLink(Vector3 size)
{
    if (size.x == 0)
        return size.y >= 3;
    else if (size.y == 0)
        return size.x >= 3;
    else // vertical roomlinks
        return min(size.x, size.y) >= RoomLayoutConfig::verticalRoomLinkWidth + 2 && max(size.x, size.y) >= RoomLayoutConfig::verticalRoomLinkLength + 2;
}
