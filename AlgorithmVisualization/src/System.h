#pragma once
#include "MouseWrapper.h"
#include "Menu.h"

class System
{
public: // Constructors, destructors, assignments in here
	System(const unsigned int windowWidth = 1024, const unsigned int windowHeight = 768);
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	System(System&&) = delete;
	System& operator=(System&&) = delete;
	~System();

public: // Public functions go in here
	void Run();

	sf::Vector2f GetMousePosition();

private: // Private functions go in here
	void HandleSFEvents();
	void Update();
	void Render();

public: // Public variables go in here
	const unsigned int windowWidth;
	const unsigned int windowHeight;

private: // Private variables go in here
	// Used to ensure only one system is present at any time
	static bool instantiated;
	// The main window
	sf::RenderWindow window;
	// Used to clamp the mouse position to the window and convert it from int to float vector
	MouseWrapper mouse;
	// Create menu
	Menu topMenu;
	// Fonts
	std::shared_ptr<sf::Font> FontPressStart = std::make_shared<sf::Font>();

	// Test shape
	sf::CircleShape shape = sf::CircleShape(100.0f);
};