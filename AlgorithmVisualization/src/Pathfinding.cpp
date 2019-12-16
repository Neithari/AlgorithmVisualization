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

void Pathfinding::ResetGrid(bool resetSpecial)
{
	// Set the whole grid to fields
	for (auto& node : grid)
	{
		node->ResetNode(resetSpecial);
	}
	// Generate new start and finish nodes if we reset the specials
	if (resetSpecial)
	{
		GenerateStartFinish();
	}
}

bool Pathfinding::Dijkstra()
{
	// Reset the delay time before we start
	ResetDelayTime();
	// Reset a possible path back to fields, reset the distance and make everything not finalized
	ResetGrid(false);

	std::shared_ptr<Node> shortestNode;
	bool nodesToExplore = true;
	
	do
	{
		// If there are no nodes left to explore and the finish is not found stop the algorithm
		if (!nodesToExplore)
		{
			return true;
		}
		nodesToExplore = false;

		int currentDistance = std::numeric_limits<int>::max();

		// Iterate over all nodes in the grid to find the one with the lowest distance
		for (auto& node : grid)
		{
			// Pick the first node with the lowest distance that's not finalized yet
			if (node->GetDistance() < currentDistance && !node->IsFinalized())
			{
				// Set to true as long as there are still nodes to explore
				nodesToExplore = true;

				// Set the currentDistance to the nodes distance to only get nodes with less distance in the following iterations
				currentDistance = node->GetDistance();
				// Set the shortestNode to the current node
				shortestNode = node;
			}
		}
		// Color the shortestNode in shortestColor
		shortestNode->ColorShortestOrAdjacent(true);

		// Update all distances to adjacent nodes of the node with the shortest distance and set the shortestNode as parent for them
		// if they are not finalized yet and if there is no shorter path.
		UpdateAdjacentDistances(shortestNode);

		// Finalize the node with the lowest distance
		shortestNode->Finalize();

		// Sleep for some time to make the visualization better
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
	} while (shortestNode != finishNode);

	// Draw path by tracing parents back from finish to start
	DrawPath();

	return true;
}

bool Pathfinding::AStar()
{
	// Reset the delay time before we start
	ResetDelayTime();
	// Reset a possible path back to fields, reset the distance and make everything not finalized
	ResetGrid(false);

	std::shared_ptr<Node> shortestNode;
	bool nodesToExplore = true;

	do
	{
		// If there are no nodes left to explore and the finish is not found stop the algorithm
		if (!nodesToExplore)
		{
			return true;
		}
		nodesToExplore = false;

		int currentDistance = std::numeric_limits<int>::max();

		// Iterate over all nodes in the grid to find the one with the lowest combined distance of distance and estimate distance
		for (auto& node : grid)
		{
			// Initialize the combinedDistance to the node distance + the estimate distance to our finish node
			int combinedDistance = node->GetDistance() + node->EstimateDistanceTo(*finishNode);
			// If the nodes distance is infinite we get a negative combinedDistance, so we need to set it to infinite.
			if (combinedDistance < 0)
			{
				combinedDistance = std::numeric_limits<int>::max();
			}
			// Pick the first node with the lowest distance that's not finalized yet
			if (combinedDistance < currentDistance && !node->IsFinalized())
			{
				// Set to true as long as there are still nodes to explore
				nodesToExplore = true;

				// Set the currentDistance to the combined distance to only get nodes with less combined distance in the following iterations
				currentDistance = combinedDistance;
				// Set the shortestNode to the current node
				shortestNode = node;
			}
		}
		// Color the shortestNode in shortestColor
		shortestNode->ColorShortestOrAdjacent(true);

		// Update all distances to adjacent nodes of the node with the shortest distance and set the shortestNode as parent for them
		// if they are not finalized yet and if there is no shorter path.
		UpdateAdjacentDistances(shortestNode);

		// Finalize the node with the lowest distance
		shortestNode->Finalize();

		// Sleep for some time to make the visualization better
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
	} while (shortestNode != finishNode);

	// Draw path by tracing parents back from finish to start
	DrawPath();

	return true;
}

bool Pathfinding::BreadthFirst()
{
	// Reset the delay time before we start
	ResetDelayTime();
	// Reset a possible path back to fields, reset the distance and make everything not finalized
	ResetGrid(false);

	std::list<std::shared_ptr<Node>> que;

	// Insert start into the que
	que.push_back(startNode);
	// Mark start as visited
	startNode->Finalize();

	// While something is inside the que...
	while (que.size() != 0)
	{
		// Stop the search if we are at the finish node
		if (que.front() == finishNode)
		{
			break;
		}
		// Color the current node
		que.front()->ColorShortestOrAdjacent(true);

		// ...get all adjacent nodes of the first element...
		auto adjacentNodes = que.front()->GetAdjacentNodes();
		// Go over every adjacent node
		for (auto& adjacentNode : adjacentNodes)
		{
			// If the adjacent node was not visited before...
			if (!adjacentNode->IsFinalized())
			{
				// ...color the current node...
				adjacentNode->ColorShortestOrAdjacent(false);
				// ...if it's no wall...
				if (!adjacentNode->IsWall())
				{
					// ...set the distance to this node to the distance of the node we are comming from + the cost to get to this node...
					adjacentNode->SetDistance(que.front()->GetDistance() + adjacentNode->GetNodeCost());
					// ...add it to the end of the que...
					que.push_back(adjacentNode);
					// ...and mark it as visited.
					adjacentNode->Finalize();
					// Set the parent to the node we are comming from
					adjacentNode->SetParent(que.front());
				}
			}
		}
		// Remove the current node from the top of the que
		que.pop_front();
		// Sleep for some time to make the visualization better
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
	}
	// Draw path by tracing parents back from finish to start
	DrawPath();

	return true;
}

bool Pathfinding::DepthFirst()
{
	// Reset the delay time before we start
	ResetDelayTime();
	// Reset a possible path back to fields, reset the distance and make everything not finalized
	ResetGrid(false);

	std::list<std::shared_ptr<Node>> stack;

	// Insert start into the que
	stack.push_back(startNode);
	// Mark start as visited
	startNode->Finalize();

	// While something is inside the que...
	while (stack.size() != 0)
	{
		// Save the current node
		auto currentNode = stack.front();
		// Remove the current node from the stack
		stack.pop_front();
		// Stop the search if we are at the finish node
		if (currentNode == finishNode)
		{
			break;
		}
		// Color the current node
		currentNode->ColorShortestOrAdjacent(true);

		// ...get all adjacent nodes of the current node...
		auto adjacentNodes = currentNode->GetAdjacentNodes();
		// Go over every adjacent node
		for (auto& adjacentNode : adjacentNodes)
		{
			// If the adjacent node was not visited before...
			if (!adjacentNode->IsFinalized())
			{
				// ...color the current node...
				adjacentNode->ColorShortestOrAdjacent(false);
				// ...if it's no wall...
				if (!adjacentNode->IsWall())
				{
					// ...add it to the stack...
					stack.push_front(adjacentNode);
					// ...and mark it as visited.
					adjacentNode->Finalize();
					// Set the parent to the node we are comming from
					adjacentNode->SetParent(currentNode);
				}
			}
		}
		// Sleep for some time to make the visualization better
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
	}
	// Draw path by tracing parents back from finish to start
	DrawPath();
	
	return true;
}

void Pathfinding::SpeedUpAlgorithm()
{
	// Lock delay time mutex
	std::lock_guard<std::recursive_mutex> lock(mtxDelayTime);
	delayTime = 0;
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
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	startNode = grid.at(start);
	finishNode = grid.at(finish);
}

void Pathfinding::UpdateAdjacentDistances(std::shared_ptr<Node> node)
{
	// Update all distances to adjacent nodes of the given node and set the node as parent for them
	// if they are not finalized yet and if there is no shorter path.
	auto adjacentNodes = node->GetAdjacentNodes();
	int minAdjacentDistance = std::numeric_limits<int>::max();

	for (auto& adjacentNode : adjacentNodes)
	{
		// Set the distance of the adjacent node to the distance of the current node + the cost to get to the adjacent node if the adjacent node is not finalized
		// and the new distance is shorter than the current distance
		int newDistance = node->GetDistance() + adjacentNode->GetNodeCost();

		if (!adjacentNode->IsFinalized() && newDistance < adjacentNode->GetDistance())
		{
			// Color the adjacentNode in adjacentColor
			adjacentNode->ColorShortestOrAdjacent(false);

			adjacentNode->SetDistance(newDistance);
			adjacentNode->SetParent(node);
		}
	}
}

void Pathfinding::DrawPath()
{
	// Draw path by tracing parents back from finish to start if there is a parent
	auto parent = finishNode->GetParent();
	if (parent)
	{
		do
		{
			parent->SetNodeType(Node::NodeType::path);
			parent = parent->GetParent();
		} while (parent->GetGridCoords() != startNode->GetGridCoords());
	}
}

void Pathfinding::ResetDelayTime()
{
	// Lock delay time mutex
	std::lock_guard<std::recursive_mutex> lock(mtxDelayTime);
	delayTime = delayTimeReset;
}
