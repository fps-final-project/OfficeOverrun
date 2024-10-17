#pragma once
#include "Drawable.hpp"
#include <DirectXMath.h>
#include "RoomCollision.hpp"


struct RoomLinkData
{
	int roomId;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	bool alongX;
};

class Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	std::vector<RoomLinkData> m_links;
	static const float wallOffset;
public:
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, const std::vector<RoomLinkData>& links = {});
	RoomCollision checkCollision(DirectX::XMFLOAT3 entityPos) const;
	bool insideRoom(DirectX::XMFLOAT3 pos) const;
	std::vector<int> getAdjacentRooms();
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};
