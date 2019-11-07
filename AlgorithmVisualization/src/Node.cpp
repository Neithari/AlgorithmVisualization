#include "pch.h"
#include "Node.h"

Node::Node(ClickOptions lable, sf::Vector2f nodePosition, NodeType type, std::pair<int, int> gridCoordinates)
	:
	ClickableObject(lable),
	type(type),
	nodeSize(17.0f, 17.0f),
	fieldColor(200,200,200),
	wallColor(255,100,0),
	startColor(0,255,100),
	finishColor(0,100,255),
	hoverColor(0, 179, 179),
	pathColor(255, 255, 100),
	shortestColor(195, 155, 255),
	adjacentColor(120, 100, 200),
	nodeShape(nodeSize),
	currentColor(GetCurrentTypeColor()),
	coords(gridCoordinates)
{
	nodeShape.setOutlineThickness(2.0f);
	nodeShape.setFillColor(currentColor);
	nodeShape.setPosition(nodePosition);
}

void Node::Update()
{
	// Check for interaction
	CheckInteraction(nodeShape);
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);
	// Update the nodeShape color to the current color
	nodeShape.setFillColor(currentColor);
}

void Node::Render(sf::RenderTarget& target) const
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	target.draw(nodeShape);
}

void Node::ResetNode(bool resetSpecial)
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

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
	// Set current color back to NodeType color
	currentColor = GetCurrentTypeColor();
}

void Node::SetNodeType(NodeType type)
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	// Change nodeCost, color and type
	switch (type)
	{
	case Node::NodeType::field:
		nodeCost = 1;
		currentColor = fieldColor;
		this->type = NodeType::field;
		break;
	case Node::NodeType::wall:
		nodeCost = std::numeric_limits<int>::max();
		currentColor = wallColor;
		this->type = NodeType::wall;
		break;
	case Node::NodeType::start:
		nodeCost = 0;
		currentColor = startColor;
		this->type = NodeType::start;
		// Set distance to 0 because we are already there
		distance = 0;
		break;
	case Node::NodeType::finish:
		nodeCost = 1;
		currentColor = finishColor;
		this->type = NodeType::finish;
		break;
	case Node::NodeType::path:
		if (this->type != NodeType::start && this->type != NodeType::finish)
		{
			currentColor = pathColor;
			this->type = NodeType::path;
		}
		break;
	default:
		std::cerr << "NodeType not declared!" << std::endl;
		break;
	}
	nodeShape.setFillColor(currentColor);
}

const std::pair<int, int>& Node::GetGridCoords() const
{
	return coords;
}

void Node::AddAdjacentNode(std::shared_ptr<Node> node)
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);
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
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	finalized = true;
}

std::vector<std::shared_ptr<Node>>& Node::GetAdjacentNodes()
{
	return adjacentNodes;
}

void Node::SetDistance(int distance)
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

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
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	parentNode = parent;
}

std::shared_ptr<Node> Node::GetParent()
{
	return parentNode;
}

void Node::ColorShortestOrAdjacent(bool shortest)
{
	if (type != NodeType::start && type != NodeType::finish)
	{
		if (shortest)
		{
			currentColor = shortestColor;
		}
		else
		{
			currentColor = adjacentColor;
		}
	}
}

int Node::EstimateDistanceTo(const Node& target) const
{
	// Get the grid coordinates of the target
	auto targetCoords = target.GetGridCoords();
	// Set xDistance to the target x - this node x
	int xDistance = targetCoords.first - coords.first;
	// Set yDistance to the target y - this node y
	int yDistance = targetCoords.second - coords.second;

	// If x or y distance are negative make it positive so we always get a positive estimateDistance
	if (xDistance < 0)
	{
		xDistance = -xDistance;
	}
	if (yDistance < 0)
	{
		yDistance = -yDistance;
	}

	// Return the estimate distance that is a combined x and y distance
	return xDistance + yDistance;
}

bool Node::IsWall() const
{
	return type == NodeType::wall;
}

void Node::IdleState()
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

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
	case Node::NodeType::path:
		nodeShape.setOutlineColor(pathColor);
		break;
	default:
		break;
	}
}

void Node::HoverState()
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	nodeShape.setOutlineColor(hoverColor);
}

void Node::PressedState()
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	// If the pressed node is a field or a path change it to a wall and vice versa
	if (type == NodeType::field || type == NodeType::path)
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
