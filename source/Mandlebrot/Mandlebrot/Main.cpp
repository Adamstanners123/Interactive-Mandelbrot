#include <iostream>
#include "RenderLoop.h"
#include "Input.h"


using namespace std;

void main(int argc, char** argv[]) {
	//Winow settings
	sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Mandelbrot", sf::Style::Close);
	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));
	//Main classes
	Input input;
	RenderLoop loop(&window, &input);
	//For delta time
	sf::Clock clock;
	float deltaTime;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				//Close window
				window.close();
				break;
			case sf::Event::KeyPressed:
				//Update input class
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				//Update input class
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					// update input class
					input.setMouseLeftDown(true);
					input.setMousePosition(event.mouseButton.x, event.mouseButton.y);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					// update input class
					input.setMouseLeftDown(false);
					loop.scaleZoom();
				}
				break;
			default:
				//Don't handle other events  
				break;
			}
		}

		// Calculate delta time. How much time has passed
		// Since it was last calculated (in seconds) and restart the clock
		deltaTime = clock.restart().asSeconds();

		loop.handleInput(deltaTime);
		loop.update();
		loop.render();
	}
}