#pragma once
#include "Button.h"

class Menu
{
public:
	explicit Menu(std::shared_ptr<sf::Font> buttonFont);

	void Update();
	void Render(sf::RenderTarget& target) const;

	void CreateButton(const ClickOptions lable, const std::string& buttonName);

private:
	void UpdateButtons();
	void AdvanceButtonPosition();

private:
	// For Button size and positioning
	std::vector<Button> buttons;
	const sf::Vector2f buttonSize = sf::Vector2f(100.0f, 30.0f);
	sf::Vector2f buttonPosition = sf::Vector2f(20.0f, 10.0f);
	static constexpr float buttonPadding = 20.0f;
	// For the font
	std::shared_ptr<sf::Font> buttonFont;
	static constexpr unsigned int buttonFontSize = 16;
};
