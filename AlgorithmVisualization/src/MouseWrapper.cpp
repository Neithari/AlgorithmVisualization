#include "pch.h"
#include "MouseWrapper.h"

MouseWrapper::MouseWrapper(const unsigned int screenWidth, const unsigned int screenHeight, const sf::Window& window)
	: screenWidth(screenWidth), screenHeight(screenHeight), window(window)
{
}

const sf::Vector2f MouseWrapper::GetPosition() const
{
	auto mousePos = sf::Mouse::getPosition(window);
	ClampToWindow(mousePos);
	return sf::Vector2f((float)mousePos.x, (float)mousePos.y);
}

void MouseWrapper::ClampToWindow(sf::Vector2i& mousePos) const
{
	if (mousePos.x < 0)
	{
		mousePos.x = 0;
	}
	if (mousePos.x > screenWidth)
	{
		mousePos.x = screenWidth;
	}
	if (mousePos.y < 0)
	{
		mousePos.y = 0;
	}
	if (mousePos.y > screenHeight)
	{
		mousePos.y = screenHeight;
	}
}
