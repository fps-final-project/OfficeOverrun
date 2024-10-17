#include "Drawable.hpp"
#include <DirectXMath.h>
#include "RoomCollision.hpp"

#ifndef ROOM_H
#define ROOM_H

class Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	static const float wallOffset;
public:
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size);
	RoomCollision isInBounds(DirectX::XMFLOAT3 entityPos) const;
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};

#endif