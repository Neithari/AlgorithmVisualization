#include "pch.h"
#include "System.h"

// Definition of mutex
std::recursive_mutex mtx;
// Used to ensure only one system is present at any time and the instance is globally accessible.
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
	// Set the backgroundcolor

	LoadFonts();
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

void System::AddClickEvent(ClickOptions lable)
{
	// Adds a click event to the que that will be processed in HandleClickEvent()
	clickEventQ.push_back(lable);
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
	// TopMenu
	topMenu.Update();

	// Grid
	grid.Update();

	HandleClickEvent();
}

void System::Render()
{
	// Clear window
	window.clear(backgroundColor);

	// render everything
	topMenu.Render(window);
	grid.Render(window);

	// Display rendered items
	window.display();
}

void System::SetupEverything()
{
	// Create buttons in topMenu
	topMenu.CreateButton(ClickOptions::reset, "Reset Grid");
	topMenu.CreateButton(ClickOptions::dijkstra, "Dijkstra's");
	topMenu.CreateButton(ClickOptions::a, "A*");
	topMenu.CreateButton(ClickOptions::breadth, "Breadth-First");
	topMenu.CreateButton(ClickOptions::depth, "Depth-First");
	topMenu.CreateButton(ClickOptions::greedy, "Greedy");
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

void System::HandleClickEvent()
{

	for (auto& e : clickEventQ)
	{
		switch (e)
		{
		case ClickOptions::dijkstra:
			algorithmFutures.push_back(std::async(std::launch::async, &Pathfinding::Dijkstra, &grid));
			break;
		case ClickOptions::a:
			algorithmFutures.push_back(std::async(std::launch::async, &Pathfinding::AStar, &grid));
			break;
		case ClickOptions::breadth:
			algorithmFutures.push_back(std::async(std::launch::async, &Pathfinding::Breadth, &grid));
			break;
		case ClickOptions::depth:
			break;
		case ClickOptions::greedy:
			break;
		case ClickOptions::reset:
			grid.ResetGrid();
			break;
		default:
			break;
		}
	}
	clickEventQ.clear();
}
