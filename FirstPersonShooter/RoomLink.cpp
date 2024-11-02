#include "pch.h"
#include "RoomLink.h"
#include "RngUtils.h"
#include "RoomLayoutConfig.h"

using namespace WorldGenerator;

RoomLink::RoomLink(const Vector3& pos, const Orientation& orientation)
    : pos(pos), orientation(orientation)
{
}

// TODO: play with rng
RoomLink RoomLink::MakeRoomLink(Vector3 pos, Vector3 size)
{
    Orientation orientation = (size.x == 0) ? YZ : ((size.y == 0)? XZ : XY);

    Vector3 linkPos = Vector3(
        pos.x + (size.x > 0 ? RngUtils::RandIntInRange(1, size.x - 2) : 0),
        pos.y + (size.y > 0 ? RngUtils::RandIntInRange(1, size.y - 2) : 0),
        orientation == XY ? pos.z + size.z : pos.z
    );

    return RoomLink(linkPos, orientation);
}

bool WorldGenerator::RoomLink::ValidBorderForRoomLink(Vector3 size)
{
    if (size.x == 0)
        return size.y >= 3;
    else if (size.y == 0)
        return size.x >= 3;
    else // vertical roomlinks
        return min(size.x, size.y) >= RoomLayoutConfig::verticalRoomLinkWidth + 2 && max(size.x, size.y) >= RoomLayoutConfig::verticalRoomLinkLength + 2;
}
