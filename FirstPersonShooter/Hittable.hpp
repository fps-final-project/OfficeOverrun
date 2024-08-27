#pragma once

class Hittable
{
public:
	virtual bool Hit(const Hittable& other) const { return false; }

protected:
	//bounding box
};

