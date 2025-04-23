#pragma once

#include"CoordinateTransformer.h"

class Camera
{
public: 
	Camera(CoordinateTransformer& ct)
		:
		ct(ct) {
	}
	Vec2 GetPos() const
	{
		return pos; 
	}

	void MoveBy(const Vec2& offset)
	{
		pos += offset; 
	}

	void MoveTo(const Vec2& pos_in)
	{
		pos = pos_in; 
	}

	void Draw(Drawable& drawable) const
	{
		drawable.Translate(-pos); 
		drawable.Scale(scale); 
		ct.Draw(drawable); 
	}

	float GetScale()
	{
		return scale; 
	}

	void SetScale(float s)
	{
		scale = s; 
	}
	
private: 
	Vec2 pos = { 0.0f, 0.0f };
	CoordinateTransformer& ct; 

	float scale = 1.0f; 
};