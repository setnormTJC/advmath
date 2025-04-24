#pragma once

#include<vector> 
#include"Vec2.h"
#include"Drawable.h"

#include<random> 

#include"Star.h"

class Entity
{
public: 
	Entity(std::vector<Vec2> model, const Vec2& pos = {0.0f, 0.0f}, Color c = Colors::Yellow)
		:
		c(c),
		pos(pos),
		model(std::move(model)) //note the std::move 
	{}
	const Vec2& GetPos() const
	{
		return pos; 
	}

	void SetPos(const Vec2& newPos)
	{
		pos = newPos; 
	}
	void TranslateBy(const Vec2& offset)
	{
		pos += offset; 
	}

	void SetScale(float s)
	{
		scale = s; 
	}

	float GetScale() const
	{
		return scale; 
	}

	Drawable GetDrawable() const
	{
		Drawable d(model, c); 

		d.Scale(scale); 
		d.Translate(pos); 

		return d; 
	}


private: 
	Color c; 
	float scale = 1.0f; 
	Vec2 pos = { 0.0f, 0.0f };
	std::vector<Vec2> model; 
	
};

class Starfield
{
private: 
	std::vector<Entity> stars; 
	Vec2 lowerLeftBoxBound = { -2000.0f, -2000.0f };
	Vec2 upperRightBoxBound = { +4000.0f, +4000.0f };

	static constexpr float MAX_OUTER_DIAMETER = 200.0f; //used to prevent overlap below...
	static constexpr float MIN_INNER_DIAMETER = 10.0f;

	int numberOfStars = 0;

	std::vector<Vec2> possibleStarSizeRatios =
	{
		Vec2(100.0f,				50.0f),
		Vec2(MAX_OUTER_DIAMETER,	50.0f),
		Vec2(100.0f,				75.0f),
		Vec2(50.0f,					MIN_INNER_DIAMETER)
	};

	std::vector<int> possibleNumbersOfFlares =
	{
		5, 6, 7, 8, 9,				17 //oddball
	};

	std::vector<Vec2> occupiedPositions;

	std::mt19937 rng;

	std::uniform_int_distribution<int> numbersOfFlaresDistribution;

	std::uniform_int_distribution<int> randomColorDistribution;

	std::uniform_real_distribution<float> positionDistribution;

	std::uniform_int_distribution<> starSizeDistribution;
	

public: 
	Starfield(int numberOfStars)
		:
		numberOfStars(numberOfStars),
		rng(std::random_device{}()),
		numbersOfFlaresDistribution(0, possibleNumbersOfFlares.size() - 1),
		randomColorDistribution(0, 255),
		positionDistribution(lowerLeftBoxBound.x, upperRightBoxBound.x),
		starSizeDistribution(0, possibleStarSizeRatios.size() - 1)

	{

	}

	std::vector<Entity> getEntities()
	{
		return stars; 
	}

	void fillEntities()
	{
		while (stars.size() <= numberOfStars)
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
			

			stars.emplace_back
			(Star::Make(randomSizeRatio.x, randomSizeRatio.y, numberOfFlares),
				randomPosition,
				Colors::MakeRGB(randomColorDistribution(rng), randomColorDistribution(rng), randomColorDistribution(rng)));
		}

	}

};