#include "Drawable.hpp"
#include <DirectXMath.h>

class Room : public Drawable
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
public:
	Room(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size);
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
};