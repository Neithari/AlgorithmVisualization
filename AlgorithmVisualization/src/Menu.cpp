#include "pch.h"
#include "Menu.h"
#include "System.h"

Menu::Menu(std::shared_ptr<sf::Font> buttonFont)
	: buttonFont(buttonFont)
{
}

void Menu::Update()
{
	UpdateButtons();
}

void Menu::Render(sf::RenderTarget& target) const
{
	for (auto& button : buttons)
	{
		button.Render(target);
	}
}

void Menu::CreateButton(const ClickOptions lable, const std::string& buttonName)
{
	buttons.emplace_back(lable, buttonPosition, buttonSize, buttonName, buttonFont, buttonFontSize);
	AdvanceButtonPosition();
}

void Menu::UpdateButtons()
{
	for (auto& button : buttons)
	{
		button.Update();
	}
}

void Menu::AdvanceButtonPosition()
{
	buttonPosition.x += buttonSize.x + buttonPadding;
}
