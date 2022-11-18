// Gerallt 2017.

#include "RenderCanvasTest.h"
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

//TODO: encapsulate render window code into RenderingManager

RenderCanvasTest::RenderCanvasTest()
{
	renderer = new Renderer();
}
RenderCanvasTest::~RenderCanvasTest()
{
	delete renderer;
}

void RenderCanvasTest::OnInit()
{
	window = new sf::RenderWindow(sf::VideoMode(dispwidth, dispheight), "Test Render Output");
	window->setFramerateLimit(30);
	renderer->window = window;

	renderer->Initilise();
}

int RenderCanvasTest::RenderLoop() const
{
	sf::Event event;
	bool running = true;

	while (running && window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();

				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		renderer->Render();

		window->display();
	}

	// Release resources ..

	return EXIT_SUCCESS;
}