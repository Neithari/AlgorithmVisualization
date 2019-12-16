#pragma once
#include "Node.h"

class Pathfinding
{
public:
	Pathfinding();
	virtual ~Pathfinding() = default;

	void Update();
	void Render(sf::RenderTarget& target) const;

	// If resetSpecial is false don't reset walls, start and finish.
	void ResetGrid(bool resetSpecial = true);

	// Pathfinding algorithms will return true when finished
	bool Dijkstra();
	bool AStar();
	bool BreadthFirst();
	bool DepthFirst();
	// Set the algorithm delay time to 0 to let it finish faster
	void SpeedUpAlgorithm();

private:
	void BuildGrid();
	// Go over every node in the grid and set it's neighbours
	void SetAdjacentNodes();
	// Generate the start and finish nodes
	void GenerateStartFinish();
	// Update the distance of all adjacent nodes of the given node
	void UpdateAdjacentDistances(std::shared_ptr<Node> node);
	// Draw path from finish to start
	void DrawPath();
	// Reset the delay time back to normal
	void ResetDelayTime();

private:
	const size_t gridSizeX = 46;
	const size_t gridSizeY = 32;

	const float padding = 22.0f;
	const float xOffset = 8.0f;
	const float yOffset = 60.0f;
	
	// Animation delay time in milliseconds
	const int delayTimeReset = 20;
	int delayTime = delayTimeReset;
	std::recursive_mutex mtxDelayTime;

	std::vector<std::shared_ptr<Node>> grid;
	// Remember start and finish for pathfinding
	std::shared_ptr<Node> startNode;
	std::shared_ptr<Node> finishNode;
};