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
	Node(ClickOptions lable, sf::Vector2f nodePosition, NodeType type);

	void Update();
	void Render(sf::RenderTarget& target) const;

private:
	// State behavior
	void IdleState();
	void HoverState();
	void PressedState();

	sf::Color GetColorForType();

private:
	// Basic colors
	const sf::Color fieldColor;
	const sf::Color wallColor;
	const sf::Color startColor;
	const sf::Color finishColor;
	const sf::Color hoverColor;

	int nodeCost;
	NodeType type;
	sf::Vector2f nodeSize;

	sf::Color nodeColor;
	sf::RectangleShape nodeShape;
};
