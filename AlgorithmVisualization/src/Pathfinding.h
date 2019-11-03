#pragma once
#include "Node.h"

class Pathfinding
{
public:
	Pathfinding();
	virtual ~Pathfinding() = default;

	void Update();
	void Render(sf::RenderTarget& target) const;
protected:
private:
	void BuildGrid();
	// Go over every node in the grid and set it's neighbours
	void SetAdjacentNodes();

private:
	const size_t gridSizeX = 46;
	const size_t gridSizeY = 32;
	const float padding = 22.0f;
	const float xOffset = 8.0f;
	const float yOffset = 60.0f;

	std::vector<std::shared_ptr<Node>> grid;

	// Random number generation
	// Get a seed from a random device, generate a pseudo random number with the seed and finaly put it into the fitting distribution
	std::random_device seed;
	std::mt19937 randomNumberGenerator;
	std::uniform_int_distribution<size_t> gridDist;
};