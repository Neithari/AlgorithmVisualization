#pragma once
#include "ClickableObject.h"

class Node : public ClickableObject
{
public:
	enum class NodeType
	{
		field, wall, start, finish
	};
public:
	Node(ClickOptions lable, sf::Vector2f nodePosition, NodeType type, std::pair<int, int> gridCoordinates);

	void Update();
	void Render(sf::RenderTarget& target) const;

	void SetNodeType(NodeType type);
	const std::pair<int, int>& GetGridCoords() const;
	void AddAdjacentNode(std::shared_ptr<Node> node);

private:
	// State behavior
	void IdleState();
	void HoverState();
	void PressedState();

	sf::Color GetCurrentTypeColor() const;

private:
	// Basic colors
	const sf::Color fieldColor;
	const sf::Color wallColor;
	const sf::Color startColor;
	const sf::Color finishColor;
	const sf::Color hoverColor;

	NodeType type;
	sf::Vector2f nodeSize;
	sf::RectangleShape nodeShape;
	// Coordinates inside the grid
	std::pair<int, int> coords;

	// For pathfinding
	int nodeCost;
	std::vector<std::shared_ptr<Node>> adjacentNodes;
};
