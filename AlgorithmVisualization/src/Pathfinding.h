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

	void Dijkstra();


private:
	void BuildGrid();
	// Go over every node in the grid and set it's neighbours
	void SetAdjacentNodes();
	// Generate the start and finish nodes
	void GenerateStartFinish();
	// Draw path from finish to start
	void DrawPath();

private:
	const size_t gridSizeX = 46;
	const size_t gridSizeY = 32;

	const float padding = 22.0f;
	const float xOffset = 8.0f;
	const float yOffset = 60.0f;

	std::vector<std::shared_ptr<Node>> grid;
	// Remember start and finish for pathfinding
	std::shared_ptr<Node> startNode;
	std::shared_ptr<Node> finishNode;
};