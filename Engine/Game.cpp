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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	ct( gfx ),
	cam( ct ),
	camCtrl( wnd.mouse,cam ),
	plank ({100.0f, 200.0f}, -380.0f, -100.0f, 290.0f), 

	//ball()

	ball({ 0.0f,-200.0f }, 15.0f, { -8.0f,32.0f })

	//ball({ 0.0f, -200.0f }, 50.0f, {10.0f, 5.0f}, Colors::Green)
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
	ball.Update(dt);
	camCtrl.Update();

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		plank.MoveFreeY(-0.2f);
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		plank.MoveFreeY(+0.2f);
	}
}


void Game::ComposeFrame()
{
	//cam.Draw(plank.GetDrawable());//error -> initial value of ref. to non-const must be l-value
	auto drawable = plank.GetDrawable(); 
	cam.Draw(drawable);

	auto ballDrawabale = ball.GetDrawable(); 
	cam.Draw(ballDrawabale);
}
