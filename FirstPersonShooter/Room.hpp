#pragma once
#include "Drawable.hpp"
#include <DirectXMath.h>
#include "RoomCollision.hpp"
#include "RoomLinkData.hpp"
#include "Entity.hpp"
#include "PropInstance.hpp"



class __declspec(dllexport) Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	std::shared_ptr<Entity> m_roomWalls;
	static const float wallOffset;

public:
	std::vector<PropInstance> m_props;
	std::vector<RoomLinkData> m_links;
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, const std::vector<RoomLinkData>& links = {});
	void setModel(Model model);
	RoomCollision checkCollision(DirectX::XMFLOAT3 entityPos) const;
	bool insideRoom(DirectX::XMFLOAT3 pos) const;

	inline DirectX::XMFLOAT3 getPosition() const { return pos; }
	inline DirectX::XMFLOAT3 getSize() const { return size; }
	inline const std::vector<RoomLinkData>& getLinks() const { return m_links; }

	std::vector<int> getAdjacentRooms() const;
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};
