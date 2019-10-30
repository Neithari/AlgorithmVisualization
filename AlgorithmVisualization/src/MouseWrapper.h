#pragma once

class MouseWrapper
{
public:
	MouseWrapper(const unsigned int screenWidth, const unsigned int screenHeight, const sf::Window& window);

	const sf::Vector2f GetPosition() const;

private:
	// Clamp the mouse to the window
	void ClampToWindow(sf::Vector2i& mousePos) const;

private:
	const unsigned int screenWidth, screenHeight;
	const sf::Window& window;
};
