#pragma once
class Hittable
{
public:
	virtual bool Hit(const Hittable& other) const = 0;

protected:
	//bounding box
};

