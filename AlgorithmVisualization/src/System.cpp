#include "pch.h"
#include "System.h"

// Used to ensure only one system is present at any time. Linker needs it here.
System System::instance;

System& System::Instance()
{
	return instance;
}

System::System(const int windowWidth, const int windowHeight)
	: windowWidth(windowWidth), windowHeight(windowHeight),
	window(sf::VideoMode(windowWidth, windowHeight), "Algorithm Visualisation"),
	topMenu(FontDustismoRoman)
{
	LoadFonts();

	// Create buttons in topMenu
	topMenu.CreateButton("Dijkstra's");
	topMenu.CreateButton("A*");
	topMenu.CreateButton("Breadth-First");
	topMenu.CreateButton("Depth-First");
	topMenu.CreateButton("Greedy");
}

void System::Run()
{
	// Used to ensure Run() is only run once
	if (runNumber > 0)
	{
		return;
	}
	runNumber++;

	// Put everything that needs setup in that function
	SetupEverything();

	// Main loop
	while (window.isOpen())
	{
		HandleSFEvents();

		Update();

		Render();
	}
}

sf::Vector2f System::GetMousePosition() const
{
	auto mousePosition = sf::Mouse::getPosition(window);
	ClampPositionToWindow(mousePosition);
	return sf::Vector2f((float)mousePosition.x, (float)mousePosition.y);
}

void System::HandleSFEvents()
{
	// Process SFML Events
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void System::Update()
{
	// Menu test code
	topMenu.Update();
	if (topMenu.SelectChanged())
	{
		auto currentSelect = topMenu.GetCurrentSelect();
		if (currentSelect.compare("Dijkstra's") == 0)
		{
			shape.setFillColor(sf::Color::Black);
		}
		else if (currentSelect.compare("A*") == 0)
		{
			shape.setFillColor(sf::Color::Cyan);
		}
		else if (currentSelect.compare("Breadth-First") == 0)
		{
			shape.setFillColor(sf::Color::Magenta);
		}
		else if (currentSelect.compare("Depth-First") == 0)
		{
			shape.setFillColor(sf::Color::Red);
		}
		else if (currentSelect.compare("Greedy") == 0)
		{
			shape.setFillColor(sf::Color::White);
		}
	}
}

void System::Render()
{
	// Clear window
	window.clear();

	// render everything
	window.draw(shape);
	topMenu.Render(window);

	// Display rendered items
	window.display();
}

void System::SetupEverything()
{
	// Test shape
	shape.setFillColor(sf::Color::Green);
}

void System::LoadFonts()
{
	if (!FontPressStart->loadFromFile("assets/fonts/press-start/prstart.ttf"))
	{
		std::cerr << "Font \"prstart.ttf\" could not be loaded" << std::endl;
	}
	if (!FontDustismoRoman->loadFromFile("assets/fonts/dustismo-roman/Dustismo_Roman.ttf"))
	{
		std::cerr << "Font \"Dustismo_Roman.ttf\" could not be loaded" << std::endl;
	}
}
