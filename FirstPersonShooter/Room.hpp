#include "Drawable.hpp"
#include <DirectXMath.h>

class Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	static const float wallOffset;
public:
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size);
	bool isInBoundsX(DirectX::XMFLOAT3 entityPos);
	bool isInBoundsY(DirectX::XMFLOAT3 entityPos);
	bool isInBoundsZ(DirectX::XMFLOAT3 entityPos);
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};