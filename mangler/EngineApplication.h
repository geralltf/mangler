#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <qwidget.h>
#include "GameSystem.h"
#include "LevelProvider.h"
#include "RenderCanvas.h"


using namespace sf;
using namespace std;

class EngineApplication : public RenderCanvas
{
public:
	GameConfig* SystemConfig;

	Texture								*tex0, *tex1, *tex2, *tex3, *tex4, *tex5, *texp1L, *texp1R;
	TextureManager						*textureManager;
	Level								*world;

	//RenderWindow						*window;
	View								*uiView;
	View								*gameView;
	View								*minimapView;
	bool								IsGameFullscreen;

protected:
	LevelProvider						*levelProvider;

protected:
	VideoMode							mode;
	Uint32								style;

	float _width;
	float _height;
	float _aspectRatio;



	Window wind; // ?

	sf::Font fontDefault;
	bool quittingApplication = false;
	bool cursorEnabled = true;
	Vector2f cursorPos;
	int frame = 0;
	float fps = 0;
	sf::Clock Clock;
	float deltaTime;
	Time time;
	sf::Event event;

	RenderStates authorRenderStates;
	RenderStates consoleRenderStates;
	Text* FPS;
	Text* author;
	Text* consoleText;
	Vector2f windowSize;
	Transform t, t2;
	float zoomLevel = 1.0f;
	int zNoiseAxisValue = 0;

	bool titleScreen = false;
	bool ctrlDown = false;
private:
	Texture *cursorTexture, *cursorTexture2;
	Sprite* cursorSprite;
	Sprite* cursorSprite2;

public:
	EngineApplication(GameConfig& SystemConfig, MainFrame* Parent, const QPoint& Position, const QSize& Size);
	EngineApplication(GameConfig& SystemConfig, MainFrame* Parent);
	~EngineApplication();

	virtual void OnInit();
	virtual void OnUpdate();

	// Run the application render loop.
	void Run();

	// Zoom the view in.
	void ZoomIn();

	// Zoom the view out.
	void ZoomOut();

	// Rotate the view in the pve + direction.
	void RotatePos();

	// Rotate the view in the neg - direction.
	void RotateNeg();

	// Window resized event; Update the aspect ratio of the window.
	void OnResize();

	// Clears the view and state for a New Level.
	void NewLevel();

	// Generates a random level using perlin noise.
	void GenerateLevel();

	// Loads the System Font Assets.
	void LoadFonts();

	// Loads and configures the system cursor.
	void LoadGCursor();

	// Load a blank Scene and the Asset folder.
	void LoadScene();

	// Renders the System Cursor.
	void RenderCursor(RenderWindow* window, Vector2f cursorPosition, float deltaTime);

	// Renders the current scene.
	void RenderScene(RenderWindow* window, float deltaTime);

	// Renders the current scene.
	void RenderObjects(RenderWindow* window, float deltaTime);

	// Makes a system icon to be used for the underlying window.
	Uint8* MakeGameIcon();

	// Calculate if the mouse position lies within the grid.
	bool IsWithinGrid(Vector2f position);

	// Move the camera to the specified world position.
	virtual void MoveCamera(Vector2f position) = 0;

	//Save the current level to file.
	virtual void Save(string fileName) = 0;

	// Load the current level from file.
	virtual void Load(string fileName) = 0;

	// Poll window events, then propagate to game and engine objects.
	virtual void PollEvents() = 0;

	// Poll window keyboard and mouse input.
	virtual void PollInput() = 0;

	// Update the Camera's position and orientation.
	virtual void UpdateCamera() = 0;

	// Exit the application. Final state changes, signaling, and cleanup before application terminates.
	virtual void Exit() = 0;

protected:

	// Draws a line between the specified points.
	void DrawLine(Vector2f a, Vector2f b, Color colour);

	// Draw a scene for a view with the game system's cursor. 
	virtual void DrawScene();

};


#endif ENGINE_APPLICATION_H