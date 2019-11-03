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
		node->Update();
	}
}

void Pathfinding::Render(sf::RenderTarget& target) const
{
	for (const auto& node : grid)
	{
		node->Render(target);
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
			auto node = std::make_shared<Node>(ClickOptions::node, nodePosition, Node::NodeType::field, std::make_pair(x, y));
			grid.push_back(std::move(node));
		}
	}

	// Go over every node in the grid and set it's neighbours
	SetAdjacentNodes();

	// Set two random nodes to start and finish
	size_t start = gridDist(randomNumberGenerator);
	size_t finish;
	// Generate a new random number till we have different start and finish
	do
	{
		finish = gridDist(randomNumberGenerator);
	} while (start == finish);
	// Set start and finish
	grid.at(start)->SetNodeType(Node::NodeType::start);
	grid.at(finish)->SetNodeType(Node::NodeType::finish);
}

void Pathfinding::SetAdjacentNodes()
{
	size_t index = 0;
	// Indices of right, left, top and bottom nodes
	size_t right = 0;
	size_t left = 0;
	size_t top = 0;
	size_t bottom = 0;

	for (size_t y = 0; y < gridSizeY; y++)
	{
		for (size_t x = 0; x < gridSizeX; x++)
		{
			// Set the index into the vector to the current x,y position
			index = y * gridSizeX + x;
			// Set the right, left, top and bottom indices to the current x,y position
			right = index + 1;
			left = index - 1;
			top = index - gridSizeX;
			bottom = index + gridSizeX;

			// Top left corner
			if (x == 0 && y == 0)
			{
				// Node(0,0) right and below are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(right));
				grid.at(index)->AddAdjacentNode(grid.at(bottom));
			}
			// Top right corner
			else if (x == gridSizeX - 1 && y == 0)
			{
				// Node(gridSizeX - 1, 0) left and bottom are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(left));
				grid.at(index)->AddAdjacentNode(grid.at(bottom));
			}
			// Bottom left corner
			else if (x == 0 && y == gridSizeY - 1)
			{
				// Node(0,gridSizeY - 1) right and top are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(right));
				grid.at(index)->AddAdjacentNode(grid.at(top));
			}
			// Bottom right corner
			else if (x == gridSizeX - 1 && y == gridSizeY - 1)
			{
				// Node(gridSizeX - 1, gridSizeY - 1) left and top are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(left));
				grid.at(index)->AddAdjacentNode(grid.at(top));
			}
			// Left side
			else if (x == 0)
			{
				// Node(0,y) right, top and bottom are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(right));
				grid.at(index)->AddAdjacentNode(grid.at(top));
				grid.at(index)->AddAdjacentNode(grid.at(bottom));
			}
			// Top side
			else if (y == 0)
			{
				// Node(x,0) right, left and bottom are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(right));
				grid.at(index)->AddAdjacentNode(grid.at(left));
				grid.at(index)->AddAdjacentNode(grid.at(bottom));
			}
			// Right side
			else if (x == gridSizeX - 1)
			{
				// Node(gridSizeX - 1, y) left, top, bottom are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(left));
				grid.at(index)->AddAdjacentNode(grid.at(top));
				grid.at(index)->AddAdjacentNode(grid.at(bottom));
			}
			// Bottom side
			else if (y == gridSizeY - 1)
			{
				// Node(x,gridSizeY - 1) right, left and top are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(right));
				grid.at(index)->AddAdjacentNode(grid.at(left));
				grid.at(index)->AddAdjacentNode(grid.at(top));
			}
			// Every other node must be in the middle
			else
			{
				// For every middle node right, left, top and bottom are adjacent
				grid.at(index)->AddAdjacentNode(grid.at(right));
				grid.at(index)->AddAdjacentNode(grid.at(left));
				grid.at(index)->AddAdjacentNode(grid.at(top));
				grid.at(index)->AddAdjacentNode(grid.at(bottom));
			}
		}
	}
}
