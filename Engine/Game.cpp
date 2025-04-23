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
#include <random>


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ct(gfx), 
	cam(ct)
{

	Vec2 lowerLeftBoxBound = { -2000.0f, -2000.0f };
	Vec2 upperRightBoxBound = { +4000.0f, +4000.0f };

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> positionDistribution
		(lowerLeftBoxBound.x,
		upperRightBoxBound.x);

	constexpr float MAX_OUTER_DIAMETER = 200.0f; //used to prevent overlap below...
	constexpr float MIN_INNER_DIAMTER = 10.0f; 

	std::vector<Vec2> possibleStarSizeRatios =
	{
		Vec2(100.0f,				50.0f),
		Vec2(MAX_OUTER_DIAMETER,	50.0f),
		Vec2(100.0f,				75.0f),
		Vec2(50.0f,					MIN_INNER_DIAMTER),
	};

	std::uniform_int_distribution<> starSizeDistribution
		(0, possibleStarSizeRatios.size() - 1);

	constexpr int NUMBER_OF_STARS = 20; 

	std::vector<int> possibleNumbersOfFlares =
	{
		5, 6, 7, 8, 9,				17 //oddball
	};

	std::uniform_int_distribution<> numbersOfFlaresDistribution(0, possibleNumbersOfFlares.size() - 1);

	std::uniform_int_distribution<> randomColorDistribution(0, 255);

	int currentStarIndex = 0; 

	std::vector<Vec2> occupiedPositions; 

	while (entities.size() <= NUMBER_OF_STARS)
	{
		Vec2 randomSizeRatio = possibleStarSizeRatios[starSizeDistribution(rng)];
		Vec2 randomPosition(positionDistribution(rng), positionDistribution(rng));
		
		//prevent overlap by checking that randomPosition is not so close to other occupied positions
		bool starOverlaps = false; 
		constexpr float overlapTolerance = 100.0f; 

		for (const auto& position : occupiedPositions)
		{
			if (std::abs(position.x - randomPosition.x) < overlapTolerance
				||
				std::abs(position.y - randomPosition.y) < overlapTolerance
				)
			
			{
				starOverlaps = true; 
				break; //look no further - an overlap was found 
			}
		}
		if (starOverlaps) continue; //"reroll" -> go no further in this loop iteration 

		occupiedPositions.push_back(randomPosition);  

		int numberOfFlares = possibleNumbersOfFlares[numbersOfFlaresDistribution(rng)];

		entities.emplace_back
		(Star::Make(randomSizeRatio.x, randomSizeRatio.y, numberOfFlares),
			randomPosition,
			Colors::MakeRGB(randomColorDistribution(rng), randomColorDistribution(rng), randomColorDistribution(rng)));	
	}


	//entities.emplace_back(Star::Make(100.0f, 50.0f), Vec2{ 460.0f, 0.0f });
	//entities.emplace_back(Star::Make(150.0f, 50.0f), Vec2{ 150.0f, 300.0f });

	//entities.emplace_back(Star::Make(350.0f, 20.0f), Vec2{ 50.0f, 300.0f });

	//entities.emplace_back(Star::Make(150.0f, 150.0f), Vec2{ 550.0f, 300.0f });

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
	const float speed = 3.0f;

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
		cam.MoveBy({ 0.0f, -speed });

	if (wnd.kbd.KeyIsPressed(VK_UP))
		cam.MoveBy({ 0.0f, +speed });

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
		cam.MoveBy({ -speed,0.0f });

	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
		cam.MoveBy({ +speed, 0.0f });

	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();

		if (e.GetType() == Mouse::Event::Type::WheelUp)
		{
			cam.SetScale(cam.GetScale() * 1.05f);
		}

		else if (e.GetType() == Mouse::Event::Type::WheelDown)
		{
			cam.SetScale(cam.GetScale() * 0.95f);
		}
	}

}


void Game::ComposeFrame()
{
	for (const auto& entity : entities)
	{
		auto drawable = entity.GetDrawable(); 

		cam.Draw(drawable);
	}

}
