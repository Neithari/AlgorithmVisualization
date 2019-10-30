#include "pch.h"
#include "Menu.h"

Menu::Menu(const unsigned int windowWidth, const unsigned int windowHeight, const MouseWrapper& mouse)
	: windowWidth(windowWidth), windowHeight(windowHeight), mouse(mouse)
{
	// Load the font
	if (!buttonFont->loadFromFile("assets/fonts/dustismo-roman/Dustismo_Roman.ttf"))
	{
		std::cerr << "Font could not be loaded" << std::endl;
	}
}

void Menu::Update()
{
	UpdateButtons();
}

void Menu::Render(sf::RenderTarget* target) const
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
		button.Update(mouse.GetPosition());
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
