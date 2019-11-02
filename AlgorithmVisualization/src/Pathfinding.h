#pragma once

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
private:
	const int gridSizeX = 46;
	const int gridSizeY = 32;
	const float padding = 22.0f;
	const float xOffset = 8.0f;
	const float yOffset = 60.0f;
	std::vector<sf::RectangleShape> grid;
};