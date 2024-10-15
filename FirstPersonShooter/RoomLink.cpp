#include "pch.h"
#include "RoomLink.h"
#include "Utils.h"

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
        pos.x + (size.x > 0 ? Utils::RandIntInRange(0, size.x - 1) : 0),
        pos.y + (size.y > 0 ? Utils::RandIntInRange(0, size.y - 1) : 0),
        pos.z + (size.z > 0 ? Utils::RandIntInRange(0, size.z - 1) : 0)
    );

    return RoomLink(linkPos, orientation);
}
