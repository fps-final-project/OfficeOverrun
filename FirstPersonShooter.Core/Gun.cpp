#include "pch.h"
#include "Gun.hpp"


Gun::Gun(AnimatedObject& object, std::string gunName) : AnimatedObject{object}, m_gunName{gunName}
{}

