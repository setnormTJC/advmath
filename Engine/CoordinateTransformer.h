#pragma once

#include<vector> 
#include"Vec2.h"
#include"Graphics.h"

class CoordinateTransformer
{
public: 
	CoordinateTransformer(Graphics& gfx)
		:
		gfx(gfx)
	{
	}

	void DrawClosedPolyline(std::vector<Vec2> verts, Color c)
	{
		Vec2 offset = { float(Graphics::ScreenWidth / 2), float (Graphics::ScreenHeight / 2)};

		for (auto& v : verts)
		{
			v.y *= -1.0f; //flip (increasing y moves up - not down) 
			v += offset;
		}
		gfx.DrawClosePolyline(verts, c); 
	}

private: 
	Graphics& gfx; 
};