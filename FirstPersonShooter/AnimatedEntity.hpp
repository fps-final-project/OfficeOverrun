#include "Hittable.hpp"
#include "Animable.hpp"

using namespace DirectX;

class AnimatedEntity : public Hittable, Animable
{
public:
	virtual void Update(float dt) = 0;
protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
};