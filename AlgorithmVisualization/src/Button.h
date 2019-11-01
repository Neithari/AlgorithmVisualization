#pragma once

enum class ButtonStates
{
	idle, hover, pressed
};
class Button
{
public:
	Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::String& buttonText, std::shared_ptr<sf::Font> font, unsigned int fontSize);

	void Update(const sf::Vector2f& mousePosition);
	void Render(sf::RenderTarget& target) const;

	const bool IsPressed() const;
	const std::string GetLabel() const;

private:
	void CheckState();

private:
	ButtonStates currentState = ButtonStates::idle;
	const std::string lable;

	std::shared_ptr<sf::Font> font;
	sf::Text buttonText;

	sf::RectangleShape shape;
	const sf::Color idleColor, hoverColor, pressedColor;
	const sf::Color idleBorderColor, hoverBorderColor, pressedBorderColor;
};
