#include <DirectXMath.h> 
#include "GunRig.h"
#include "Camera.hpp"

class Player : public Drawable
{
public:
	Player();
	void Update(float dt);
	void updateVelocity(float dt);
	void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	std::unique_ptr<GunRig>& getGunRig();
	DirectX::XMFLOAT3 getPostition();

	void setAcceleration(DirectX::XMFLOAT3 acc);
	
private:
	DirectX::XMFLOAT3 m_position, m_velocity, m_acceleration;
	std::unique_ptr<GunRig> m_gunRig;
};