#pragma once
#include "AnimatedObject.hpp"

class Gun : public AnimatedObject
{
public:
	Gun(AnimatedObject& object, std::string gunName);


	inline std::string GetGunName() const { return m_gunName; }
private:
	std::string m_gunName;
};

