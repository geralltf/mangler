//#include "GlRenderTest.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "GameApplication.h"
#include "MathHelpers.h"
#include "Common.h"
#include "LevelProvider.h"
#include "SFML/OpenGL.hpp"
#include "NoiseHelpers.h"


using namespace std;

GameApplication::GameApplication(GameConfig& config, MainFrame* Parent) : EngineApplication(config, Parent)
{

}

void GameApplication::OnInit()
{
	EngineApplication::OnInit();

	// LOAD SCENE
	LoadScene();
	LoadFonts();
	LoadGameUI();


	FPS = new sf::Text("0", fontDefault, SystemConfig->fontsizelarge);
	author = new Text("", fontDefault, SystemConfig->fontsizemed);
	consoleText = new Text("", fontDefault, SystemConfig->fontsizesmall);
	FPS->setFillColor(SystemConfig->fontColour);
	author->setFillColor(SystemConfig->fontColour);

	levelProvider = new LevelProvider(SystemConfig, textureManager, world);
	levelProvider->defaultBackground = tex5; // HACK

	if (cursorEnabled)
	{
		LoadGCursor();
	}

	// GAME
	Load(SystemConfig->saveFile); // Load level

								  //OpenGL::Initialize();
}

void GameApplication::OnUpdate()
{
	EngineApplication::OnUpdate();

	{
		// FPS
		time = Clock.getElapsedTime();
		deltaTime = time.asMilliseconds();
		fps = 1.f / time.asSeconds();
		Clock.restart();

		// EVENT SYSTEM
		PollEvents();
		PollInput();

		// UPDATE STATUS
		stringstream ss;
		ss << FloatToString(static_cast<int>(fps)) << " FPS";

		FPS->setString(ss.str());
		author->setString(SystemConfig->Game["build"]["author"]);

		// CAMERA MOVEMENT
		UpdateCamera();
	}

	// RENDER SCENE
	this->clear();

	// COMPUTE PHYSICS
	//timeStep = 1 / time.asSeconds(); // uncomment to sync physics with current game framerate
	UpdatePhysics();

	{
		// Draw the Game View
		setView(*gameView);
		DrawScene();
		RenderObjects(this, deltaTime);

		// Render test player:
		RenderPlayer();

		// Draw the Mini-map
		setView(*minimapView);
		DrawScene();
		RenderPlayer();
	}

	{
		// Draw the UI
		setView(*uiView);
		RenderUI(this, deltaTime);

		// UI.
		draw(*FPS);

		windowSize = Vector2f(getSize().x, getSize().y);

		t.translate(windowSize.x - SystemConfig->fpspos.x, windowSize.y - SystemConfig->fpspos.y);
		authorRenderStates.transform = t;
		draw(*author, authorRenderStates);

		t2.translate(SystemConfig->consolePos.x, windowSize.y - SystemConfig->consolePos.y);
		consoleRenderStates.transform = t2;
		draw(*consoleText, consoleRenderStates);

	}

	//OpenGL::glRender();

	frame++;
}

// Initilise Box2D Physics.
void GameApplication::InitPhysics()
{
	gravity = b2Vec2(SystemConfig->physGravity.x, SystemConfig->physGravity.y); // Define the gravity vector.

	bworld = new b2World(gravity); // Construct a world object, which will hold and simulate the rigid bodies.
	bworld->SetContinuousPhysics(true);
	bworld->SetAllowSleeping(true);

	// Define the dynamic body. We set its position and call the body factory.
	bodyDef = new b2BodyDef();
	bodyDef->type = b2_dynamicBody;
	bodyDef->position.Set(world->cameraPosition.x, world->cameraPosition.y);
	bodyDef->gravityScale = -20;
	bodyDef->fixedRotation = true;

	body = bworld->CreateBody(bodyDef);
	body->SetBullet(true);// ensure player doesnt fall through the world at fixed updates

						  // Define another box shape for our dynamic body.
	dynamicBox = new b2PolygonShape();
	dynamicBox->SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.

	fixtureDef = new b2FixtureDef();
	fixtureDef->shape = dynamicBox;

	// Set the box restitution so theres no bounciness; 100% velocity lost after collision.
	fixtureDef->restitution = 0.0f;


	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef->density = 1.0f;

	// Override the default friction.
	fixtureDef->friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(fixtureDef);

	cout << "Physics [initilised]" << endl;
}

// Poll window events, then propagate to game and engine objects.
void GameApplication::PollEvents()
{
	char chr = *"\0";

	while (this->pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
			//RenderWindow::close();
			break;

		case Event::Resized:
			OnResize();
			break;
		case Event::MouseWheelScrolled:

			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
			{
				//std::cout << "wheel type: vertical" << std::endl;
				//std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
				//std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
				//std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;

				std::cout << event.mouseWheelScroll.delta << std::endl;

				// generate and select a level piece

				if (event.mouseWheelScroll.delta > 0)
				{
					ZoomIn();
				}
				if (event.mouseWheelScroll.delta < 0)
				{
					ZoomOut();
				}

			}


			break;
		case Event::TextEntered:
			//TODO: complete widget controls and event system

			if (event.text.unicode < 128)
			{
				chr = static_cast<char>(event.text.unicode);
			}

			KeyPressed(event.text.unicode, chr);

			break;
		case Event::MouseButtonPressed:

			MousePressed(event.mouseButton.button, Vector2f(event.mouseButton.x, event.mouseButton.y));

			break;
		case Event::MouseButtonReleased:

			MouseReleased(event.mouseButton.button, Vector2f(event.mouseButton.x, event.mouseButton.y));

			break;
		default:
			break;
		}
	}
}

// Poll window keyboard and mouse input.
void GameApplication::PollInput()
{
	Vector2i mousePosition;

	// KEYBOARD INPUT
	if (Keyboard::isKeyPressed(SystemConfig->KEY_QUIT))
	{
		//RenderWindow::close();
	}

	ctrlDown = Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl);

	if (ctrlDown)
	{
		if (Keyboard::isKeyPressed(Keyboard::Add))
		{
			RotatePos();
		}
		if (Keyboard::isKeyPressed(Keyboard::Subtract))
		{
			RotateNeg();
		}
	}
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::Add))
		{
			ZoomIn();
		}
		if (Keyboard::isKeyPressed(Keyboard::Subtract))
		{
			ZoomOut();
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::P))
	{
		// Restart the application to go in to the editor.
		//TODO: Need to notify the player that they are quitting.
		SystemConfig->RestartApplication = true;
		SystemConfig->Options["editor"] = true;
		quittingApplication = true;

		RenderWindow::close();
		QCoreApplication::quit();
	}

	// MOUSE INPUT
	mousePosition = Mouse::getPosition(*this);

	// map pixel position to gameView world space coordinates
	sf::Vector2f worldPos = this->mapPixelToCoords(mousePosition, *gameView);

	cursorPos = Vector2f(worldPos.x, worldPos.y);

	// Update LevelEditor
	MouseMoved(cursorPos);
}

//Loads the UI and Widgets.
void GameApplication::LoadGameUI()
{

}

// Renders the UI and Widgets.
void GameApplication::RenderUI(sf::RenderWindow* window, float deltaTime)
{

}

// Update the Camera's position and orientation.
void GameApplication::UpdateCamera()
{
	// CAMERA and PLAYER MOVEMENT
	Vector2f direction = Vector2f(0, 0);
	b2Vec2 b2direction = b2Vec2(0, 0);
	float speed = 1.0f;
	float speedMov = 100000.0f;
	float jumpSpeed = 14000000000;

	if (Keyboard::isKeyPressed(SystemConfig->GAME_KEY_PLAYER_LEFT))
	{
		direction.x = 1;
		b2direction.x = -1;

		playerSprite->setTexture(*texp1L);
	}
	if (Keyboard::isKeyPressed(SystemConfig->GAME_KEY_PLAYER_RIGHT))
	{
		direction.x = 1;
		b2direction.x = 1;

		playerSprite->setTexture(*texp1R);
	}
	if (Keyboard::isKeyPressed(SystemConfig->GAME_KEY_PLAYER_JUMP))
	{
		b2Vec2 offset = b2Vec2(10, 10);

		const b2Vec2 worldCenter = body->GetWorldCenter();
		const b2Vec2 center = b2Vec2(worldCenter.x + (offset.x), worldCenter.y + offset.y);

		b2Vec2 jumpForce = b2Vec2(1, 1);

		jumpForce.x *= b2direction.x;
		jumpForce.y *= -1;
		jumpForce *= jumpSpeed;

		const b2Vec2 F = b2Vec2(jumpForce.x, jumpForce.y);

		//jumpForce = b2Vec2(0, -1 * jumpSpeed);

		//body->ApplyForce(jumpForce, center, true);

		body->ApplyLinearImpulse(F, center, true);
	}

	if (direction.x != 0 || direction.y != 0)
	{
		// MOVE PLAYER
		b2Vec2 force = b2Vec2(b2direction.x * speedMov, 0);

		const b2Vec2 worldCenter = body->GetWorldCenter();
		const b2Vec2 center = b2Vec2(worldCenter.x + (b2direction.x * 10), 0);

		//body->SetTransform(pos, body->GetAngle());

		body->ApplyForce(force, center, true);
		//body->ApplyLinearImpulse(force, center, true);
	}

	// CAMERA always follows player
	MoveCamera(playerSprite->getPosition());
}

// Update the Box2D Physics.
void GameApplication::UpdatePhysics()
{

	// Instruct the world to perform a single step of simulation.
	// It is generally best to keep the time step and iterations fixed.
	bworld->Step(timeStep, velocityIterations, positionIterations);

	// Now update the position and angle of the player body.
	b2Vec2 position = body->GetPosition();
	float32 angle = body->GetAngle();
	//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	Vector2f pos = Vector2f(position.x, position.y);
	playerSprite->setPosition(pos);
	playerSprite->setRotation(angle);
}

// Load a player into the world for testing.
void GameApplication::LoadPlayer()
{
	playerSprite = new Sprite();
	playerSprite->setScale(SystemConfig->playerScale);
	playerSprite->setTexture(*texp1R);
}

// Render the test player sprite.
void GameApplication::RenderPlayer()
{
	Vector2f halfSize = playerSprite->getScale();
	halfSize.x = (halfSize.x * playerSprite->getTexture()->getSize().x) * 0.5f;
	halfSize.y = (halfSize.y * playerSprite->getTexture()->getSize().y) * 0.5f;

	DrawLine(Vector2f(playerSprite->getPosition().x + halfSize.x, playerSprite->getPosition().y + halfSize.y), cursorPos, Color::Red);


	draw(*playerSprite);
}

// Create a collidable object given its specified position and size.
Collidable* GameApplication::createCollidable(Vector2f pos, Vector2f size)
{
	Collidable* collidable = new Collidable();
	b2Body* wallBody;
	b2PolygonShape* wallBox;
	b2BodyDef* wallBodyDef;
	//float kPixelsPerMeter = 32.0f;
	float kPixelsPerMeter = 1.0f;

	wallBodyDef = new b2BodyDef();
	wallBox = new b2PolygonShape(); // define the shape of the wall body.

	wallBodyDef->position.Set(pos.x / kPixelsPerMeter, pos.y / kPixelsPerMeter); // Define the wall body.

																				 // Call the body factory which allocates memory for the wall body
																				 // from a pool and creates the ground box shape (also from a pool).
																				 // The body is also added to the world.
	wallBody = bworld->CreateBody(wallBodyDef);


	// The extents are the half-widths of the box.
	wallBox->SetAsBox(size.x / 2, size.y / 2);
	//wallBox->SetAsBox(size.x, size.y);

	// Add the wall fixture to the wall body.
	wallBody->CreateFixture(wallBox, 0.0f);

	collidable->wallBody = wallBody;
	collidable->wallBodyDef = wallBodyDef;
	collidable->wallBox = wallBox;
	collidable->size = size;
	collidable->position = pos;


	return collidable;
}

void GameApplication::Save(string fileName) {}

// Load the current level from file.
void GameApplication::Load(string fileName)
{
	cout << "Loading " << fileName << endl;

	WorldSettings settings;
	settings.cellOffset = SystemConfig->cellOffset;
	settings.cellScale = SystemConfig->cellScale;
	settings.gridPosition = SystemConfig->gridPosition;

	world = levelProvider->Load(fileName, settings);

	InitPhysics(); // Box2D
	LoadPlayer(); // load test player

				  // Build a collision box for each 'wall' sprite in the grid.
				  // i.e. find all the mountain textures..  

	if (collidables != nullptr)
		delete collidables;

	collidables = new vector<Collidable*>();

	for (int i = 0; i < world->rows * world->columns; i++)
	{
		Sprite* sp = (*world->cells)[i];
		const Texture* tex = sp->getTexture();

		if (tex == tex3) // mountain.
		{
			Vector2f pos = sp->getPosition();
			Vector2f size = sp->getScale();
			size.x *= tex->getSize().x;
			size.y *= tex->getSize().y;

			pos.x -= (size.x / 2);
			pos.y -= (size.y / 2);

			Collidable* collidable = createCollidable(pos, size);

			collidables->push_back(collidable);
		}
	}
}

// Callback MousePressed event from the owning window.
void GameApplication::MousePressed(Mouse::Button button, Vector2f mousePos)
{
	if (button == Mouse::Button::Left)
	{
		mousePosOnPressed = mousePos;
		mouseReleased = false;
	}

}

// Callback MouseReleased event from the owning window.
void GameApplication::MouseReleased(Mouse::Button button, Vector2f mousePos)
{
	if (button == Mouse::Button::Left) {
		//HACK: hardcoded to left mouse button
		mousePosOnReleased = mousePos;
		mouseReleased = true;

	}
}



// Callback MouseMoved event from the owning window.
void GameApplication::MouseMoved(Vector2f mousePosition) // bug: this code is outdated
{
	mouseDelta = mousePosition - prevMousePos;



	// Is the mouse cursor within the grid?
	if (IsWithinGrid(mousePosition))
	{
		// transform mouse position to grid-space
		cellPos = TransformToGridSpace(SystemConfig->gridPosition, SystemConfig->cellOffset, mousePosition);

		// given cellPos find index into grid
		spriteIndex = GetMatrixIndex(cellPos.x, cellPos.y, this->world->columns);

		if (Mouse::isButtonPressed(Mouse::Button::Left)) //HACK: hardcoded to left mouse button
		{

		}
	}

	prevMousePos = mousePosition;
}

// Move the camera to the specified world position.
void GameApplication::MoveCamera(Vector2f position)
{
	gameView->setCenter(position);
}

// Callback KeyPressed event from the owning window.
void GameApplication::KeyPressed(Uint32 unicodeChar, char chr)
{

}

// Exit the application. Final state changes, signaling, and cleanup before application terminates.
void GameApplication::Exit()
{
	if (!quittingApplication)
	{
		RenderWindow::close();

		quittingApplication = true;
	}
}

// Post state changes and signaling, issue final cleanup of object memory when application quits. 
GameApplication::~GameApplication()
{
	if (cursorEnabled)
	{
		this->setMouseCursorVisible(true);
	}

	delete collidables;
	delete levelProvider;
}