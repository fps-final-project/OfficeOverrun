#pragma once
#include "AnimatedEntity.hpp"
#include "ActionTypes.hpp"
#include "Room.hpp"
#include "SourceVoice.hpp"
#include <x3daudio.h>
#include "Path.h"
#include <list> 

class Pathfinder;

class  __declspec(dllexport) Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedModel> model);

	Action Update(float dt, std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos);
	virtual void Update(float dt) override;
	void takeDamage(int damage);
	
	inline std::shared_ptr<X3DAUDIO_EMITTER> getEmitter() const { return m_emitter; }
	inline SourceVoice* getSound() { return m_sound.get(); }
	inline SourceVoice* getDamageSound() { return m_damageSound.get(); }
	inline bool isDead() const { return health <= 0; }
	inline bool inCloseProximity() const { return pathToPlayer.playerVisible; }
	
	Path pathToPlayer;


	friend class EnemyBuilder;
private:

	int health;
	int damage;
	float speed;
	float radius;
	std::shared_ptr<SourceVoice> m_sound, m_damageSound;
	std::shared_ptr<X3DAUDIO_EMITTER> m_emitter;

	const float rotationSpeed = 4.f;
	float targetRotation;

	static std::vector<X3DAUDIO_DISTANCE_CURVE_POINT> s_soundCurvePoints;
	static X3DAUDIO_DISTANCE_CURVE s_soundCurve;

	float GetRotationIncrement();
	float AdjustAngleToPositive(float angle);
	XMVECTOR GetDirection();
};

