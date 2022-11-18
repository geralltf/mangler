#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "EngineApplication.h"
#include "TextureManager.h"
#include "Collidable.h"

class GameApplication : public EngineApplication
{
private:

private:
	float moveSpeed = 5.0f;

	// Box2D Physics.
	b2Vec2 gravity;
	b2World* bworld;
	b2Body* groundBody;
	b2BodyDef* groundBodyDef;
	b2PolygonShape* groundBox;
	b2BodyDef* bodyDef;
	b2PolygonShape* dynamicBox;
	b2Body* body;
	b2FixtureDef* fixtureDef;

	// World collidables.
	vector<Collidable*>* collidables = nullptr;

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 2;

	Sprite* playerSprite; // test player.


	Vector2i cellPos;
	int spriteIndex = -1;
	int zNoiseAxisValue = 0;
	Vector2f mousePosOnPressed;
	Vector2f mousePosOnReleased;
	bool mouseReleased;
	Vector2f prevMousePos = Vector2f(0, 0);
	Vector2f mouseDelta;
public:

	GameApplication(GameConfig& config, MainFrame* Parent);
	~GameApplication();

	void OnInit();
	void OnUpdate();

	// Save the current level to file.
	void Save(string fileName) override;

	// Load the current level from file.
	void Load(string fileName) override;

public:

	// Poll window events, then propagate to game and engine objects.
	void PollEvents() override;

	// Poll window keyboard and mouse input.
	void PollInput() override;

	// Update the Camera's position and orientation.
	void UpdateCamera() override;

	// Loads the UI and Widgets.
	void LoadGameUI();

	// Renders the UI and Widgets.
	void RenderUI(RenderWindow* window, float deltaTime);

	// Initilise Box2D Physics.
	void InitPhysics();

	// Update the Box2D Physics.
	void UpdatePhysics();

	// Load a player into the world for testing.
	void LoadPlayer();

	// Render the test player sprite.
	void RenderPlayer();

	// Callback MousePressed event from the owning window.
	void MousePressed(Mouse::Button button, Vector2f mousePos);

	// Callback MouseReleased event from the owning window.
	void MouseReleased(Mouse::Button button, Vector2f mousePos);

	// Callback MouseMoved event from the owning window.
	void MouseMoved(Vector2f mousePosition);

	// Callback KeyPressed event from the owning window.
	void KeyPressed(Uint32 unicodeChar, char chr);

	// Move the camera to the specified world position.
	void MoveCamera(Vector2f position);

	// Exit the application. Final state changes, signaling, and cleanup before application terminates.
	void Exit() override;

private:
	// Create a collidable object given its specified position and size.
	Collidable* createCollidable(Vector2f pos, Vector2f size);
};

#endif