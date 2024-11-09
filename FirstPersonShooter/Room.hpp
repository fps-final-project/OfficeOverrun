#pragma once
#include "Drawable.hpp"
#include <DirectXMath.h>
#include "RoomCollision.hpp"
#include "RoomLinkData.hpp"
#include "Entity.hpp"



class Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	std::unique_ptr<Entity> m_roomWalls;
	static const float wallOffset;

public:
	std::vector<RoomLinkData> m_links;
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, const std::vector<RoomLinkData>& links = {});
	void setModel(Model model);
	RoomCollision checkCollision(DirectX::XMFLOAT3 entityPos) const;
	bool insideRoom(DirectX::XMFLOAT3 pos) const;

	inline DirectX::XMFLOAT3 getPosition() { return pos; }
	inline DirectX::XMFLOAT3 getSize() { return size; }
	inline std::vector<RoomLinkData> getLinks() { return m_links; }

	std::vector<int> getAdjacentRooms();
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};
