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

void Pathfinding::ResetGrid()
{
	// Set the whole grid to fields
	for (auto& node : grid)
	{
		node->ResetNode();
	}
	// Generate the start and finish nodes
	GenerateStartFinish();
}

void Pathfinding::Dijkstra()
{
	/// TODO: Wenn blockiert dann muss abgebrochen werden!!!!!!!!
	std::shared_ptr<Node> currentNode = startNode;
	std::shared_ptr<Node> shortestNode;
	std::vector<std::shared_ptr<Node>> finalizedNodes;
	int currentDistance = std::numeric_limits<int>::max();
	bool allExplored = false;
	// Reserve enough space in the finalized nodes
	finalizedNodes.reserve(grid.size());
	
	do
	{		
		allExplored = true;
		// Iterate over all nodes in the grid to find the one with the lowest distance
		for (auto& node : grid)
		{
			// Pick the first node with the lowest distance that's not finalized yet
			if (node->GetDistance() < currentDistance && !node->IsFinalized())
			{
				// Set the currentDistance to the nodes distance to only get nodes with less distance in the following iterations
				currentDistance = node->GetDistance();
				// Set the shortestNode to the current node
				shortestNode = node;
				allExplored = false;
			}
		}

		// Update all distances to adjacent nodes of the node with the shortest distance
		auto adjacentNodes = shortestNode->GetAdjacentNodes();
		int minAdjacentDistance = std::numeric_limits<int>::max();
		for (auto& adjacentNode : adjacentNodes)
		{
			// Set the distance of the adjacent node to the distance of the current node + the cost to get to the adjacent node if the adjacent node is not finalized
			if (!adjacentNode->IsFinalized())
			{
				int newDistance = shortestNode->GetDistance() + adjacentNode->GetNodeCost();
				if (newDistance < adjacentNode->GetDistance())
				{
					adjacentNode->SetDistance(newDistance);
					adjacentNode->SetParent(shortestNode);
				}
			}
		}
		// Finalize the node with the lowest distance and add it to the finalized nodes
		shortestNode->Finalize();
		finalizedNodes.push_back(shortestNode);

		// Set the currentNode to shortestNode
		currentNode = shortestNode;
		// Set the current distance to be the distance of the shortest node + two steps
		currentDistance = shortestNode->GetDistance() + 2 * finishNode->GetNodeCost();
	} while (!allExplored);	

	// Draw path by tracing parents back from finish to start
	auto parent = finishNode->GetParent();
	do
	{
		parent->SetNodeType(Node::NodeType::path);
		parent = parent->GetParent();
	} while (parent->GetGridCoords() != startNode->GetGridCoords());
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
	GenerateStartFinish();
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

void Pathfinding::GenerateStartFinish()
{
	// Random number generation
	// Get a seed from a random device, generate a pseudo random number with the seed and finaly put it into the fitting distribution
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_int_distribution<size_t> gridDist(0, gridSizeX * gridSizeY - 1);

	// Set the start to a random node between 0 and 
	size_t start = gridDist(randomNumberGenerator);
	size_t finish;
	// Generate a new random number till we have different start and finish
	do
	{
		finish = gridDist(randomNumberGenerator);
	} while (start == finish);

	// Set start and finish NodeTypes
	grid.at(start)->SetNodeType(Node::NodeType::start);
	grid.at(finish)->SetNodeType(Node::NodeType::finish);
	// Save start and finish nodes for easy access
	startNode = grid.at(start);
	finishNode = grid.at(finish);
}
