#include "pch.h"
#include "Node.h"

Node::Node(ClickOptions lable, sf::Vector2f nodePosition, NodeType type, std::pair<int, int> gridCoordinates)
	:
	ClickableObject(lable),
	type(type),
	nodeSize(17.0f, 17.0f),
	fieldColor(sf::Color::White),
	wallColor(sf::Color::Red),
	startColor(sf::Color::Green),
	finishColor(sf::Color::Blue),
	hoverColor(0, 179, 179),
	nodeShape(nodeSize),
	coords(gridCoordinates)
{
	nodeShape.setOutlineThickness(2.0f);
	nodeShape.setFillColor(GetCurrentTypeColor());
	nodeShape.setPosition(nodePosition);
}

void Node::Update()
{
	CheckInteraction(nodeShape);
}

void Node::Render(sf::RenderTarget& target) const
{
	target.draw(nodeShape);
}

void Node::SetNodeType(NodeType type)
{
	// Change nodeCost, color and type
	switch (type)
	{
	case Node::NodeType::field:
		nodeCost = 1;
		nodeShape.setFillColor(fieldColor);
		this->type = NodeType::field;
		break;
	case Node::NodeType::wall:
		nodeCost = std::numeric_limits<int>::max();
		nodeShape.setFillColor(wallColor);
		this->type = NodeType::wall;
		break;
	case Node::NodeType::start:
		nodeCost = 0;
		nodeShape.setFillColor(startColor);
		this->type = NodeType::start;
		break;
	case Node::NodeType::finish:
		nodeCost = 0;
		nodeShape.setFillColor(finishColor);
		this->type = NodeType::finish;
		break;
	default:
		std::cerr << "NodeType not declared!" << std::endl;
		break;
	}
}

const std::pair<int, int>& Node::GetGridCoords() const
{
	return coords;
}

void Node::AddAdjacentNode(std::shared_ptr<Node> node)
{
	adjacentNodes.push_back(node);
}

void Node::IdleState()
{
	// Set the outline color back to the type color when idle
	switch (type)
	{
	case Node::NodeType::field:
		nodeShape.setOutlineColor(fieldColor);
		break;
	case Node::NodeType::wall:
		nodeShape.setOutlineColor(wallColor);
		break;
	case Node::NodeType::start:
		nodeShape.setOutlineColor(startColor);
		break;
	case Node::NodeType::finish:
		nodeShape.setOutlineColor(finishColor);
		break;
	default:
		break;
	}
}

void Node::HoverState()
{
	nodeShape.setOutlineColor(hoverColor);
}

void Node::PressedState()
{
	TestAdjacentNodes();
	// If the pressed node is a field change it to a wall and vice versa
	if (type == NodeType::field)
	{
		SetNodeType(NodeType::wall);
	}
	else if (type == NodeType::wall)
	{
		SetNodeType(NodeType::field);
	}
}

sf::Color Node::GetCurrentTypeColor() const
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

void Node::TestAdjacentNodes()
{
	for (auto& node : adjacentNodes)
	{
		node->nodeShape.setFillColor(sf::Color::Blue);
	}
}
