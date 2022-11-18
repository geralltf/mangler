#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include <SFML/Graphics.hpp>
#include <CL/cl.h>
#include "Compute.h"
#include "Renderer.h"

using namespace std;


class RenderCanvasTest
{
private:

	Renderer* renderer;

	Compute* CL;

	//resolution of the window
	const int dispwidth = 1280;
	const int dispheight = 720;

	sf::RenderWindow* window;


public:

	RenderCanvasTest();
	~RenderCanvasTest();
	void OnInit();
	int RenderLoop() const;
};

#endif