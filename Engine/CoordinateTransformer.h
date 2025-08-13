#pragma once
#include <vector>
#include "Vec2.h"
#include "Graphics.h"
#include "Drawable.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer( Graphics& gfx )
		:
		gfx( gfx )
	{}
	void Draw( Drawable& drawable ) const
	{
		const Vec2 offset = { float( Graphics::ScreenWidth / 2 ),float( Graphics::ScreenHeight / 2 ) };/*
		drawable.ScaleIndependent( 1.0f,-1.0f );
		drawable.Translate( offset );*/

		Mat3 transformation = Mat3::Translation(offset.x, offset.y) *
			Mat3::ScaleIndependent(1.0f, -1.0f);

		drawable.ApplyTransformation
		(
			transformation
		);


		drawable.Render( gfx );
	}
private:
	Graphics& gfx;
};