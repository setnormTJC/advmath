/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Star.h"
#include "ChiliMath.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	ct( gfx ),
	cam( ct ),
	camCtrl( wnd.mouse,cam ),
	plank( { 100.0f,200.0f },-380.0f,-100.0f,290.0f ),
	spawn( balls,15.0f,{ 0.0f,-250.0f },-100.0f,25.0f,150.0f,2.0f )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();

	for( auto& ball : balls )
	{
		const auto plankVector = plank.GetPlankSurfaceVector();
		// rotate plank vector by -pi/2 to get plank "normal"
		const auto plankNormal = Vec2{ plankVector.y,-plankVector.x };
		const auto plankPts = plank.GetPoints();
		const auto ballPos = ball.GetPos();

		if( plankNormal * ball.GetVel() < 0.0f )
		{
			if( DistancePointLine( plankPts.first,plankPts.second,ballPos ) < ball.GetRadius() )
			{
				const Vec2 w = plank.GetPlankSurfaceVector().GetNormalized();
				const Vec2 v = ball.GetVel();
				ball.SetVel( w * (v * w) * 2.0f - v );
				collideSound.Play();
			}
		}

		ball.Update( dt );
	}
	spawn.Update( dt );

	camCtrl.Update();
	if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
	{
		plank.MoveFreeY( -2.0f );
	}
	if( wnd.kbd.KeyIsPressed( VK_UP ) )
	{
		plank.MoveFreeY( 2.0f );
	}

	// remove balls that are far out
	const auto new_end = std::remove_if( balls.begin(),balls.end(),
		[this]( const Ball& b )
	{
		return b.GetPos().LenSq() > sq( maxBallDistance );
	} );
	balls.erase( new_end,balls.end() );
}

void Game::ComposeFrame()
{
	auto plankDrawable = plank.GetDrawable(); 
	cam.Draw( plankDrawable );
	
	
	for( const auto& ball : balls )
	{
		auto ballDrawable = ball.GetDrawable(); 
		cam.Draw( ballDrawable );
	}
}
