#pragma once
#include "Button.h"
#include "MouseWrapper.h"

class Menu
{
public:
	Menu(const unsigned int windowWidth, const unsigned int windowHeight, const MouseWrapper& mouse);

	void Update();
	void Render(sf::RenderTarget* target) const;

	void CreateButton(const std::string& buttonName);

	// Returns true if a menu button got pressed
	const bool SelectChanged() const;
	// Get the last selected menu option
	const std::string& GetCurrentSelect() const;

private:
	void UpdateButtons();
	void AdvanceButtonPosition();

private:
	const int windowWidth, windowHeight;
	const MouseWrapper& mouse;
	// For Button size and positioning
	std::vector<Button> buttons;
	const sf::Vector2f buttonSize = sf::Vector2f(100.0f, 30.0f);
	sf::Vector2f buttonPosition = sf::Vector2f(20.0f, 10.0f);
	static constexpr float buttonPadding = 20.0f;
	// For the font
	std::shared_ptr<sf::Font> buttonFont = std::make_shared<sf::Font>();
	static constexpr unsigned int buttonFontSize = 16;
	// For the selection process
	std::string currentSelect;
	bool newSelect = false;
};
