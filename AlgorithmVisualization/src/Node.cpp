#include "pch.h"
#include "Node.h"

Node::Node(ClickOptions lable, sf::Vector2f nodePosition, NodeType type)
	:
	ClickableObject(lable),
	type(type),
	nodeSize(20.0f, 20.0f),
	fieldColor(sf::Color::White),
	wallColor(sf::Color::Red),
	startColor(sf::Color::Green),
	finishColor(sf::Color::Blue),
	nodeColor(GetColorForType()),
	nodeShape(nodeSize)
{
	nodeShape.setFillColor(nodeColor);
	nodeShape.setPosition(nodePosition);
}

void Node::Update()
{
	currentState = ClickableStates::idle;
	if (nodeShape.getGlobalBounds().contains(System::Instance().GetMousePosition()))
	{
		currentState = ClickableStates::hover;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			currentState = ClickableStates::pressed;
		}
	}
	CheckState();
}

void Node::Render(sf::RenderTarget& target) const
{
	target.draw(nodeShape);
}

void Node::IdleState()
{
}

void Node::HoverState()
{
}

void Node::PressedState()
{
	if (type == NodeType::field)
	{
		// Increase nodeCost to represent a wall, change the color and type to wall
		nodeCost = 0;
		nodeColor = wallColor;
		nodeShape.setFillColor(nodeColor);
		type = NodeType::wall;
	}
	else if (type == NodeType::wall)
	{
		// Increase nodeCost to represent a wall, change the color and type to wall
		nodeCost = 1;
		nodeColor = fieldColor;
		nodeShape.setFillColor(nodeColor);
		type = NodeType::field;
	}
}

sf::Color Node::GetColorForType()
{
	// No need for break because we return the color
	switch (type)
	{
	case Node::NodeType::field:
		return fieldColor;
	case Node::NodeType::wall:
		return wallColor;
	case Node::NodeType::start:
		return startColor;
	case Node::NodeType::finish:
		return finishColor;
	default:
		return sf::Color::Magenta;
	}
}
