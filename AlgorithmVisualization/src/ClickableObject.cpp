#include "pch.h"
#include "ClickableObject.h"


ClickableObject::ClickableObject(ClickOptions lable) : lable(lable)
{
}

void ClickableObject::CheckInteraction(sf::Shape& shape)
{
	// Lock mutex
	std::lock_guard<std::recursive_mutex> lock(mtx);

	currentState = ClickableStates::idle;
	if (shape.getGlobalBounds().contains(System::Instance().GetMousePosition()))
	{
		currentState = ClickableStates::hover;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			currentState = ClickableStates::pressed;
		}
	}

	CheckState();
}

void ClickableObject::CheckState()
{
	switch (currentState)
	{
	case ClickableStates::idle:
		IdleState();
		break;
	case ClickableStates::hover:
		HoverState();
		break;
	case ClickableStates::pressed:
		// This will prevent perma activation when click is held down
		if (lastState != currentState)
		{
			PressedState();
		}
		break;
	default:
		std::cerr << "Something went wrong with the currentState";
		break;
	}
	lastState = currentState;
}
