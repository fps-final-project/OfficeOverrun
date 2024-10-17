#include "Drawable.hpp"
#include <DirectXMath.h>
#include "RoomCollision.hpp"

#ifndef ROOM_H
#define ROOM_H

struct RoomLinkData
{
	int roomId;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
};

class Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	std::vector<RoomLinkData> m_links;
	static const float wallOffset;
public:
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, const std::vector<RoomLinkData>& links = {});
	RoomCollision isInBounds(DirectX::XMFLOAT3 entityPos) const;
	std::vector<int> getAdjacentRooms();
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};

#endif