#include "pch.h"
#include "GeneratedRoom.h"

using namespace WorldGenerator;

bool GeneratedRoom::IsAdjacent(GeneratedRoom room)
{
    if (Utils::LinesIntersect1D(pos.x, pos.x + size.x, room.pos.x, room.pos.x + room.size.x)
        && Utils::LinesIntersect1D(pos.y, pos.y + size.y, room.pos.y, room.pos.y + room.size.y)
        && Utils::LinesIntersect1D(pos.z, pos.z + size.z, room.pos.z, room.pos.z + room.size.z))
    {
        // Check if rooms are not adjacent by diagonal (only one must be true)
        return Utils::LinesIntersectByEnds1D(pos.x, pos.x + size.x, room.pos.x, room.pos.x + room.size.x)
            + Utils::LinesIntersectByEnds1D(pos.y, pos.y + size.y, room.pos.y, room.pos.y + room.size.y)
            + Utils::LinesIntersectByEnds1D(pos.z, pos.z + size.z, room.pos.z, room.pos.z + room.size.z)
            == 1;
    }
    else
        return false;
}

bool WorldGenerator::GeneratedRoom::ValidRoomLink(GeneratedRoom room)
{
    if (!IsAdjacent(room))
        return false;
    auto border = ComputeBorders(room);
    if (!RoomLink::ValidSizeForRoomLink(std::get<1>(border)))
        return false;
    else
        return true;
}

bool WorldGenerator::GeneratedRoom::IsAbove(GeneratedRoom room)
{
    return pos.z > room.pos.z;
}

bool WorldGenerator::GeneratedRoom::IsBelow(GeneratedRoom room)
{
    return pos.z < room.pos.z;
}

bool WorldGenerator::GeneratedRoom::IsSameLevel(GeneratedRoom room)
{
    return pos.z == room.pos.z;
}

bool WorldGenerator::GeneratedRoom::IsZeroFloor()
{
    return pos.z == 0;
}

// Returns pos and size turple
std::tuple<Vector3, Vector3> GeneratedRoom::ComputeBorders(GeneratedRoom room)
{
    Vector3 borderPos, borderSize;
    // Borders in axis YZ
    if (Utils::LinesIntersectByEnds1D(pos.x, pos.x + size.x, room.pos.x, room.pos.x + room.size.x))
    {
        int xPos = (pos.x == room.pos.x + room.size.x) ? pos.x : pos.x + size.x;
        auto yInt = Utils::ComputeLinesIntersection1D(pos.y, pos.y + size.y, room.pos.y, room.pos.y + room.size.y);
        auto zInt = Utils::ComputeLinesIntersection1D(pos.z, pos.z + size.z, room.pos.z, room.pos.z + room.size.z);

        borderPos = Vector3(xPos, std::get<0>(yInt), std::get<0>(zInt));
        borderSize = Vector3(0, std::get<1>(yInt), std::get<1>(zInt));
    }
    // Borders in axis XZ
    if (Utils::LinesIntersectByEnds1D(pos.y, pos.y + size.y, room.pos.y, room.pos.y + room.size.y))
    {
        int yPos = (pos.y == room.pos.y + room.size.y) ? pos.y : pos.y + size.y;
        auto xInt = Utils::ComputeLinesIntersection1D(pos.x, pos.x + size.x, room.pos.x, room.pos.x + room.size.x);
        auto zInt = Utils::ComputeLinesIntersection1D(pos.z, pos.z + size.z, room.pos.z, room.pos.z + room.size.z);

        borderPos = Vector3(std::get<0>(xInt), yPos, std::get<0>(zInt));
        borderSize = Vector3(std::get<1>(xInt), 0, std::get<1>(zInt));
    }
    // Borders in axis XY
    if (Utils::LinesIntersectByEnds1D(pos.z, pos.z + size.z, room.pos.z, room.pos.z + room.size.z))
    {
        int zPos = (pos.z == room.pos.z + room.size.z) ? pos.z : pos.z + size.z;
        auto xInt = Utils::ComputeLinesIntersection1D(pos.x, pos.x + size.x, room.pos.x, room.pos.x + room.size.x);
        auto yInt = Utils::ComputeLinesIntersection1D(pos.y, pos.y + size.y, room.pos.y, room.pos.y + room.size.y);

        borderPos = Vector3(std::get<0>(xInt), std::get<0>(yInt), zPos);
        borderSize = Vector3(std::get<1>(xInt), std::get<1>(yInt), 0);
    }

    return std::tuple<Vector3, Vector3>(borderPos, borderSize);
}
