#pragma once
#include "Menu.h"
#include "ClickableObject.h"
#include "Pathfinding.h"

enum class ClickOptions
{
	dijkstra, a, breadth, depth, greedy, node, reset
};

class System
{
private: // Constructor is private to permit more than one construction
	System(const int windowWidth = 1024, const int windowHeight = 768);
public: // Constructors, destructors, assignments in here
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	System(System&&) = delete;
	System& operator=(System&&) = delete;

public: // Public functions go in here
	// Access the only System instance
	static System& Instance();
	// Use only from main()
	void Run();

	// Gets the adjusted mouse position for our window
	sf::Vector2f GetMousePosition() const;
	// Adds a ClickEvent to the cue
	void AddClickEvent(ClickOptions lable);

private: // Private functions go in here
	void HandleSFEvents();
	void Update();
	void Render();
	// Clamp a position to the window. THIS WILL CHANGE THE GIVEN POSITION
	template <typename T>
	void ClampPositionToWindow(T& position) const;
	// Put everything that needs setup in that function
	void SetupEverything();
	void LoadFonts();
	// Process the click events from CklickableObjects
	void HandleClickEvent();

public: // Public variables go in here
	const int windowWidth;
	const int windowHeight;

private: // Private variables go in here
	// Used to ensure only one system is present at any time
	static System instance;
	// Used to ensure Run() is only run once
	int runNumber;
	// The main window
	sf::RenderWindow window;
	// Fonts
	std::shared_ptr<sf::Font> FontPressStart = std::make_shared<sf::Font>();
	std::shared_ptr<sf::Font> FontDustismoRoman = std::make_shared<sf::Font>();
	// Vector to store every click event for processing
	std::vector<ClickOptions> clickEventQ;

	// Create menu
	Menu topMenu;

	// Test shape
	sf::CircleShape shape = sf::CircleShape(100.0f);
	// Test pathfinding
	Pathfinding grid;
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
