#include "pch.h"
#include "Object.hpp"

Object::Object(std::shared_ptr<AssimpModel> model) : Entity{ model }
{
}
