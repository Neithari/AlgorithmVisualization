#pragma once
#include "ClickableObject.h"
class Button : public ClickableObject
{
public:
	Button(const ClickOptions lable, const sf::Vector2f& position, const sf::Vector2f& size, const sf::String& buttonText, std::shared_ptr<sf::Font> font, unsigned int fontSize);
	~Button() = default;

	void Update();
	void Render(sf::RenderTarget& target) const;

private:
	// State behavior
	void IdleState();
	void HoverState();
	void PressedState();

private:
	std::shared_ptr<sf::Font> font;
	sf::Text buttonText;

	sf::RectangleShape shape;
	const sf::Color idleColor, hoverColor, pressedColor;
	const sf::Color idleBorderColor, hoverBorderColor, pressedBorderColor;
};
