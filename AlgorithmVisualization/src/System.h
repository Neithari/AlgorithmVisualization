#pragma once
#include "Menu.h"

class System
{
private:
	System(const unsigned int windowWidth = 1024, const unsigned int windowHeight = 768);
public: // Constructors, destructors, assignments in here
	static System& Instance();
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	System(System&&) = delete;
	System& operator=(System&&) = delete;

public: // Public functions go in here
	// Use only from main()
	void Run();

	// Gets the adjusted mouse position for our window
	sf::Vector2f GetMousePosition() const;

private: // Private functions go in here
	void HandleSFEvents();
	void Update();
	void Render();
	// Clamp a position to the window. THIS WILL CHANGE THE GIVEN POSITION
	template <typename T>
	void ClampPositionToWindow(T& position) const;

public: // Public variables go in here
	const int windowWidth;
	const int windowHeight;

private: // Private variables go in here
	// Used to ensure only one system is present at any time
	static System instance;
	// The main window
	sf::RenderWindow window;
	// Create menu
	Menu topMenu;
	// Fonts
	std::shared_ptr<sf::Font> FontPressStart = std::make_shared<sf::Font>();

	// Test shape
	sf::CircleShape shape = sf::CircleShape(100.0f);
};

// Clamp a position to the window. THIS WILL CHANGE THE GIVEN POSITION
template<typename T>
inline void System::ClampPositionToWindow(T& position) const
{
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > windowWidth)
	{
		position.x = windowWidth;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	if (position.y > windowHeight)
	{
		position.y = windowHeight;
	}
}