/// AlgorithmVisualization is used to visualize different programming algorithms like pathfinding or sorting.

#include "pch.h"
#include "Button.h"
#include "MouseWrapper.h"
#include "Menu.h"

int main()
{
	// Set window size here
	constexpr int windowWidth = 800;
	constexpr int windowHeight = 600;
	
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");
	
	MouseWrapper mouse(windowWidth, windowHeight, window);

	// Create menu and buttons
	Menu menu(windowWidth, windowHeight, mouse);
	menu.CreateButton("Dijkstra's");
	menu.CreateButton("A*");
	menu.CreateButton("Breadth-First");
	menu.CreateButton("Depth-First");
	menu.CreateButton("Greedy");

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	std::shared_ptr<sf::Font> FontPressStart = std::make_shared<sf::Font>();
	if (!FontPressStart->loadFromFile("assets/fonts/press-start/prstart.ttf"))
	{
		std::cerr << "Font could not be loaded" << std::endl;
		return 0;
	}
	sf::Vector2f buttonPos(302.0f, 200.0f);
	sf::Vector2f buttonSize(200.0f, 40.0f);
	Button button1(buttonPos, buttonSize, "Press Me", FontPressStart, 20);

	int reset = 0;
	while (window.isOpen())
	{
		// Process SFML Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		// Button test code
		button1.Update(mouse.GetPosition());
		if (button1.IsPressed())
		{
			shape.setFillColor(sf::Color::Blue);
		}
		if (reset > 5000)
		{
			reset = 0;
			shape.setFillColor(sf::Color::Green);
		}
		reset++;

		// Menu test code
		menu.Update();
		if (menu.SelectChanged())
		{
			auto currentSelect = menu.GetCurrentSelect();
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

		// Draw
		window.clear();
		window.draw(shape);
		button1.Render(&window);
		menu.Render(&window);
		window.display();
	}

	return 0;
}
