#pragma once

#include"Entity.h"
#include"Star.h"

class Ball : public Entity
{
public: 
	Ball(Vec2 pos, float radius, Vec2 vel, Color c = Colors::Red)
		:
		Entity(Star::Make(radius, radius, 4), pos, c), 
		radius(radius), 
		vel(vel)
	{
	};
	//cute use of same radius for BOTH inner and outer radius (gives a circle) 

	float GetRadius() const
	{
		return radius;
	}
	Vec2 GetVel() const
	{
		return vel;
	}
	void SetVel(const Vec2& vel_in)
	{
		vel = vel_in;
	}
	void Update(float dt)
	{
		TranslateBy(vel * dt);
	}

private: 
	float radius = 10.0f; 
	Vec2 vel = { -8.0f,32.0f };
};