#include "pch.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
	:
	randomNumberGenerator(seed()),
	gridDist(0, gridSizeX * gridSizeY)
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
	// Reserve enough space inside the vector
	grid.reserve(gridSizeX * gridSizeY);
	// Generade the grid
	for (int y = 0; y < (int)gridSizeY; y++)
	{
		for (int x = 0; x < (int)gridSizeX; x++)
		{
			sf::Vector2f nodePosition((float)x * padding + xOffset, (float)y * padding + yOffset);
			Node node(ClickOptions::node, nodePosition, Node::NodeType::field);
			grid.push_back(std::move(node));
		}
	}
	// Set two random nodes to start and finish
	size_t start = gridDist(randomNumberGenerator);
	size_t finish;
	// Generate a new random number till we have different start and finish
	do
	{
		finish = gridDist(randomNumberGenerator);
	} while (start == finish);
	// Set start and finish
	grid.at(start).SetNodeType(Node::NodeType::start);
	grid.at(finish).SetNodeType(Node::NodeType::finish);
}
