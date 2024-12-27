#pragma once
#include "AnimatedObject.hpp"

class Gun : public AnimatedObject
{
public:
	Gun(AnimatedObject& object, std::string gunName);

private:
	std::string m_gunName;
};

