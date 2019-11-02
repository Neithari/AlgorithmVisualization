#include "pch.h"
#include "Button.h"
#include "System.h"

Button::Button(const ClickOptions lable, const sf::Vector2f& position, const sf::Vector2f& size, const sf::String& buttonText, std::shared_ptr<sf::Font> font, unsigned int fontSize)
	:
	ClickableObject(lable),
	font(font),
	buttonText(buttonText, *this->font, fontSize),
	shape(size),
	idleColor(0, 128, 128),
	hoverColor(0, 179, 179),
	pressedColor(0, 230, 230),
	idleBorderColor(0, 179, 179),
	hoverBorderColor(0, 230, 230),
	pressedBorderColor(0, 179, 179)
{
	// Setup the shape
	shape.setPosition(position);
	shape.setFillColor(idleColor);
	shape.setOutlineThickness(3.0f);
	shape.setOutlineColor(idleBorderColor);

	// Setup the text to be centered
	auto textBounds = this->buttonText.getLocalBounds();
	auto buttonSize = shape.getSize();
	// Text does have an offset in textBounds.left/top and must be subtracted to align properly
	this->buttonText.setPosition(position.x + (buttonSize.x - textBounds.width) / 2 - textBounds.left, position.y + (buttonSize.y - textBounds.height) / 2 - textBounds.top);
}

void Button::Update()
{
	CheckInteraction(shape);
}

void Button::Render(sf::RenderTarget& target) const
{
	target.draw(shape);
	target.draw(buttonText);
}

void Button::IdleState()
{
	shape.setFillColor(idleColor);
	shape.setOutlineColor(idleBorderColor);
}

void Button::HoverState()
{
	shape.setFillColor(hoverColor);
	shape.setOutlineColor(hoverBorderColor);
}

void Button::PressedState()
{
	shape.setFillColor(pressedColor);
	shape.setOutlineColor(pressedBorderColor);

	System::Instance().AddClickEvent(lable);
}
