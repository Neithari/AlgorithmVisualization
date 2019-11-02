#include "pch.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
	BuildGrid();
}

void Pathfinding::Update()
{
}

void Pathfinding::Render(sf::RenderTarget& target) const
{
	for (const auto& g : grid)
	{
		target.draw(g);
	}
}

void Pathfinding::BuildGrid()
{
	grid.reserve((size_t)gridSizeX * (size_t)gridSizeY);
	sf::Vector2f patchSize(20.0f, 20.0f);
	for (int y = 0; y < gridSizeY; y++)
	{
		for (int x = 0; x < gridSizeX; x++)
		{
			sf::RectangleShape patch(patchSize);
			patch.setPosition((float)x * padding + xOffset, (float)y * padding + yOffset);
			grid.push_back(std::move(patch));
		}
	}
}
