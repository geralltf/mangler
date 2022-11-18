// Gerallt 2017.

#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "IOHelpers.h"

class Renderer
{
private:
	GLuint program = 0, vao = 0;

public:
	sf::RenderWindow* window;

	Renderer(): window(nullptr)
	{
		// Do nothing here.
	}
	~Renderer()
	{
		// Do nothing here.
	}

	void Initilise();

	void LoadObject();

	void LoadShader();

	void Render();
};

#endif