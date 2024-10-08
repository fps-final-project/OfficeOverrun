#include <DirectXMath.h> 
#include "GunRig.h"
#include "Camera.hpp"

class Player : public Drawable
{
public:
	Player();
	void Update(float dt);
	void updateVelocity(DirectX::XMFLOAT3 deltaV);
	void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	std::unique_ptr<GunRig>& getGunRig();
	DirectX::XMFLOAT3 getPostition();
	
private:
	DirectX::XMFLOAT3 m_position, m_velocity;
	std::unique_ptr<GunRig> m_gunRig;
};