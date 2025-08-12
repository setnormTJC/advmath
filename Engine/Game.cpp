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

#include"ChiliMath.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	ct( gfx ),
	cam( ct ),
	camCtrl( wnd.mouse,cam ),
	plank ({100.0f, 200.0f}, -380.0f, -100.0f, 290.0f), 
	spawn(balls, 15.0f, { 0.0f,-250.0f }, -100.0f, 25.0f, 150.0f, 0.4f)
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
	
	const auto plankPts = plank.GetPoints();

	for (auto& ball : balls)
	{
		if (DistancePointLine(plankPts.first, plankPts.second, ball.GetPos())
			< ball.GetRadius())
		{
			const Vec2 w = plank.GetPlankSurfaceVector().GetNormalized(); 
			const Vec2 v = ball.GetVel(); 

			const auto dotProduct = v.operator*(w); //dot product operation here
									//since a Vec2 is in parentheses 

			ball.SetVel(w * dotProduct * 2.0f - v); 

			//collideSound.Play(); //collision sound is terribly loud at the moment
		}

		ball.Update(dt);
	}

	spawn.Update(dt);

	camCtrl.Update();

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		plank.MoveFreeY(-0.2f);
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		plank.MoveFreeY(+0.2f);
	}


	//remove far away balls: 
	auto new_end = std::remove_if(balls.begin(), balls.end(),
		[this](const Ball& b)
		{
			return b.GetPos().LenSq() > maxBallDistance * maxBallDistance;
		}
		);

	balls.erase(new_end, balls.end());

}


void Game::ComposeFrame()
{
	//cam.Draw(plank.GetDrawable());//error -> initial value of ref. to non-const must be l-value
	auto drawable = plank.GetDrawable(); 
	cam.Draw(drawable);

	for (auto& ball : balls)
	{
		auto ballDrawabale = ball.GetDrawable();
		cam.Draw(ballDrawabale);
	}
}
