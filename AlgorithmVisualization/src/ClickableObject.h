#pragma once
// Forward declare ClickOptions
enum class ClickOptions;

// Base class for everything thats clickable
class ClickableObject
{
public:
	explicit ClickableObject(ClickOptions lable);
	virtual ~ClickableObject() = default;

	virtual void Update() = 0;
	virtual void Render(sf::RenderTarget& target) const = 0;

protected:
	// For the current state of the clickable object
	enum class ClickableStates
	{
		idle, hover, pressed
	};
	// Switch over the ClickableStates 
	void CheckState();
	// Every child must implement their own state behavior
	virtual void IdleState() = 0;
	virtual void HoverState() = 0;
	virtual void PressedState() = 0;

protected:
	const ClickOptions lable;
	ClickableStates currentState = ClickableStates::idle;
	ClickableStates lastState;
};

