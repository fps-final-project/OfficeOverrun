#include <DirectXMath.h> 
#include "GunRig.h"
#include "Camera.hpp"
#include "RoomCollision.hpp"

class Player : public Drawable
{
public:
	Player();
	void jump();
	void Update(float dt);
	void updateVelocity(float dt);
	void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	std::unique_ptr<GunRig>& getGunRig();
	DirectX::XMFLOAT3 getPostition();

	void setAcceleration(DirectX::XMFLOAT3 acc);
	void handleRoomCollision(const RoomCollision& collisionData);
private:
	DirectX::XMFLOAT3 m_position, m_velocity, m_acceleration;
	std::unique_ptr<GunRig> m_gunRig;
	bool m_isOnGround;
};