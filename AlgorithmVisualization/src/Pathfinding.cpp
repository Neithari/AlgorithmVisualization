#include "pch.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
	BuildGrid();
}

void Pathfinding::Update()
{
	for (auto& node : grid)
	{
		node.Update();
	}
}

void Pathfinding::Render(sf::RenderTarget& target) const
{
	for (const auto& node : grid)
	{
		node.Render(target);
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
			sf::Vector2f nodePosition((float)x * padding + xOffset, (float)y * padding + yOffset);
			Node node(ClickOptions::node, nodePosition, Node::NodeType::field);
			grid.push_back(std::move(node));
		}
	}
}
