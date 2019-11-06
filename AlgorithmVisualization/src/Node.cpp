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
	pathColor(sf::Color::Yellow),
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

void Node::ResetNode(bool resetSpecial)
{
	// If we dont want to reset walls, start and finish then...
	if (!resetSpecial)
	{
		// ...reset the path to be a field...
		if (type == NodeType::path)
		{
			// Set node to field
			SetNodeType(NodeType::field);
		}
		// ...and reset the distance of everything exept the start to infinite.
		if (type != NodeType::start)
		{
			// Reset distance to infinite
			distance = std::numeric_limits<int>::max();
		}
	}
	// If we want to reset everything then...
	else
	{
		// ...set node to field...
		SetNodeType(NodeType::field);
		// ...and reset distance to infinite.
		distance = std::numeric_limits<int>::max();
	}
	// Set node to not finalized
	finalized = false;
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
		// Set distance to 0 because we are already there
		distance = 0;
		break;
	case Node::NodeType::finish:
		nodeCost = 1;
		nodeShape.setFillColor(finishColor);
		this->type = NodeType::finish;
		break;
	case Node::NodeType::path:
		if (this->type != NodeType::start && this->type != NodeType::finish)
		{
			nodeShape.setFillColor(pathColor);
			this->type = NodeType::path;
		}
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

const int Node::GetDistance() const
{
	return distance;
}

const int Node::GetNodeCost() const
{
	return nodeCost;
}

const bool Node::IsFinalized() const
{
	return finalized;
}

void Node::Finalize()
{
	finalized = true;
}

std::vector<std::shared_ptr<Node>>& Node::GetAdjacentNodes()
{
	return adjacentNodes;
}

void Node::SetDistance(int distance)
{
	// Set the distance to "infinite" if the node is a wall
	if (type == NodeType::wall)
	{
		this->distance = std::numeric_limits<int>::max();
	}
	else
	{
		this->distance = distance;
	}
}

void Node::SetParent(std::shared_ptr<Node> parent)
{
	parentNode = parent;
}

std::shared_ptr<Node> Node::GetParent()
{
	return parentNode;
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
