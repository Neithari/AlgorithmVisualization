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

void Menu::CreateButton(const std::string& buttonName)
{
	buttons.emplace_back(buttonPosition, buttonSize, buttonName, buttonFont, buttonFontSize);
	AdvanceButtonPosition();
}

const bool Menu::SelectChanged() const
{
	return newSelect;
}

const std::string& Menu::GetCurrentSelect() const
{
	return currentSelect;
}

void Menu::UpdateButtons()
{
	for (auto& button : buttons)
	{
		newSelect = false;
		
		button.Update(System::Instance().GetMousePosition());
		if (button.IsPressed())
		{
			newSelect = true;
			currentSelect = button.GetLabel();
			return;
		}
	}
}

void Menu::AdvanceButtonPosition()
{
	buttonPosition.x += buttonSize.x + buttonPadding;
}
