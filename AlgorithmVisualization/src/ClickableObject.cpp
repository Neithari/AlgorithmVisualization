#include "pch.h"
#include "ClickableObject.h"


ClickableObject::ClickableObject(ClickOptions lable) : lable(lable)
{
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
		PressedState();
		break;
	default:
		std::cerr << "Something went wrong with the currentState";
		break;
	}
}
