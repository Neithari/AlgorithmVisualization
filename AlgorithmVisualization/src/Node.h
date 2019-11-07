#pragma once
#include "ClickableObject.h"

class Node : public ClickableObject
{
public:
	enum class NodeType
	{
		field, wall, start, finish, path
	};
public:
	Node(ClickOptions lable, sf::Vector2f nodePosition, NodeType type, std::pair<int, int> gridCoordinates);

	void Update();
	void Render(sf::RenderTarget& target) const;
	// If resetSpecial is false don't reset walls, start and finish.
	void ResetNode(bool resetSpecial = true);
	void SetNodeType(NodeType type);
	const std::pair<int, int>& GetGridCoords() const;
	void AddAdjacentNode(std::shared_ptr<Node> node);

	// Used for pathfinding
	const int GetDistance() const;
	const int GetNodeCost() const;
	// check if the node is already finalized
	const bool IsFinalized() const;
	// Set finalized to true and set the NodeType to path
	void Finalize();
	std::vector<std::shared_ptr<Node>>& GetAdjacentNodes();
	void SetDistance(int distance);
	void SetParent(std::shared_ptr<Node> parent);
	std::shared_ptr<Node> GetParent();
	// For visualization
	// Color the node in shortestColor if true or in adjacentColor if false
	void ColorShortestOrAdjacent(bool shortest);
	// Estimate the distance to the given object
	int EstimateDistanceTo(const Node& target) const;

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
	const sf::Color pathColor;
	const sf::Color shortestColor;
	const sf::Color adjacentColor;
	
	NodeType type;
	sf::Vector2f nodeSize;
	sf::RectangleShape nodeShape;
	sf::Color currentColor;
	// Coordinates inside the grid
	std::pair<int, int> coords;

	// For pathfinding
	int nodeCost = 1;
	// Set distance to infinite uppon creation
	int distance = std::numeric_limits<int>::max();
	std::vector<std::shared_ptr<Node>> adjacentNodes;
	bool finalized = false;
	std::shared_ptr<Node> parentNode;
};
