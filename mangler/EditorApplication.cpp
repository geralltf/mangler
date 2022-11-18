#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "MathHelpers.h"
#include "NoiseHelpers.h"
#include "Common.h"
#include "LevelProvider.h"
#include "Editor.h"
#include "EditorApplication.h"
#include "TextureToolWidget.h"
#include "DrawToolWidget.h"
#include "VectorLib.h"

using namespace std;

EngineApplication* EditorContext;

EditorApplication::EditorApplication(GameConfig& config, MainFrame* Parent, const QPoint& Position, const QSize& Size)
	: EngineApplication(config, Parent, Position, Size)
{
	Init_Perlin_Noisy();
}

void EditorApplication::OnInit()
{
	EngineApplication::OnInit(); // The Base Class actually initilises the window for us.

								 // LOAD SCENE
	LoadScene();
	LoadFonts();
	LoadEditorUI();

	if (toolW1 != nullptr)
	{
		toolW1->texture = tex0;
	}

	FPS = new Text("0", fontDefault, SystemConfig->fontsizelarge);
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
}

void EditorApplication::OnUpdate()
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
		ss << FloatToString(static_cast<int>(fps)) << " ";
		ss << "FPS  (" << cursorPos.x << "," << cursorPos.y << ") ";
		ss << "cxy: (" << cellPos.x << "," << cellPos.y << ") ";
		ss << "fid: " << frame << " ";
		ss << "si: " << spriteIndex << " ";

		FPS->setString(ss.str());
		author->setString(SystemConfig->Game["build"]["author"]);

		// CAMERA MOVEMENT
		UpdateCamera();

	}

	// RENDER SCENE
	clear(sf::Color(0, 0, 0));

	{
		// Draw the Game View.
		setView(*gameView);
		DrawScene();

		// Draw the Mini-map.
		setView(*minimapView);
		DrawScene();
	}

	{
		// Draw the UI.
		setView(*uiView);

		hud->Render();
		titleScreen = hud->Visible;

		if (hud->Visible)
		{
			auto mpi = Mouse::getPosition(*this);

			RenderCursor(this, Vector2f(mpi.x, mpi.y), deltaTime);
		}

		RenderEditorUI(this, deltaTime);

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

	frame++;
}

void EditorApplication::DrawScene()
{
	//EngineApplication::DrawScene();

	RenderScene(this, deltaTime);

	RenderObjects(this, deltaTime);

	if (cameraSprite != nullptr)
		draw(*cameraSprite);

	if (cursorEnabled)
	{
		// Show Game Cursor.
		RenderCursor(this, cursorPos, deltaTime);

		// Hide System Cursor.
		this->setMouseCursorVisible(false);
	}

}

// Poll window events, then propagate to game and engine objects.
void EditorApplication::PollEvents()
{
	char chr = *"\0";
	Vector2i mousePositionI;
	Vector2f mousePosition;
	Vector2f worldPos;

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

			if (event.text.unicode < 128)
			{
				chr = static_cast<char>(event.text.unicode);
			}

			KeyPressed(event.text.unicode, chr);

			break;
		case Event::MouseButtonPressed:

			mousePositionI = Vector2i(event.mouseButton.x, event.mouseButton.y);
			mousePosition = Vector2f(event.mouseButton.x, event.mouseButton.y);
			worldPos = this->mapPixelToCoords(mousePositionI, *gameView);

			MousePressed(event.mouseButton.button, mousePosition, worldPos);

			break;
		case Event::MouseButtonReleased:
			mousePositionI = Vector2i(event.mouseButton.x, event.mouseButton.y);
			mousePosition = Vector2f(event.mouseButton.x, event.mouseButton.y);
			worldPos = this->mapPixelToCoords(mousePositionI, *gameView);

			MouseReleased(event.mouseButton.button, mousePosition, worldPos);

			break;
		default:
			break;
		}

		// Pass the event to all the TGUI widgets
		hud->PropagateEvent(event);
	}
}

// Save the current level to file.
void EditorApplication::Save(string file)
{
	cout << "Saving " << file << endl;

	WorldSettings settings;
	settings.cellOffset = Vector2f(SystemConfig->cellOffset);
	settings.cellScale = Vector2f(SystemConfig->cellScale);
	settings.gridPosition = Vector2f(SystemConfig->gridPosition);

	levelProvider->Save(file, world, settings);
}

// Load the current level from file.
void EditorApplication::Load(string file)
{
	cout << "Loading " << file << endl;

	WorldSettings settings;
	settings.cellOffset = SystemConfig->cellOffset;
	settings.cellScale = SystemConfig->cellScale;
	settings.gridPosition = SystemConfig->gridPosition;

	world = levelProvider->Load(file, settings);

	// Set default camera position.
	Vector2f cameraPos = Vector2f(world->cameraPosition.x, world->cameraPosition.y);
	gameView->setCenter(cameraPos);

	// Show camera test object.
	auto cameraTexture = textureManager->GetTextureByName(TEX_CAMERA);
	cameraSprite = new Sprite();
	cameraSprite->setTexture(*cameraTexture);
	Vector2u size = cameraTexture->getSize();
	cameraSprite->setOrigin(size.x / 2, size.y / 2);
	MoveCamera(cameraPos);
}

void EditorApplication::Save()
{
	hud->OpenSave();
}

void EditorApplication::Load()
{
	hud->OpenLoad();
}

// Poll window keyboard and mouse input.
void EditorApplication::PollInput()
{
	Vector2i mousePosition;

	// KEYBOARD INPUT
	if (Keyboard::isKeyPressed(SystemConfig->KEY_QUIT))
	{
		//RenderWindow::close();
	}

	if (!hud->Visible)
	{
		if (Keyboard::isKeyPressed(SystemConfig->KEY_LOAD))
		{
			Load();
		}
		if (Keyboard::isKeyPressed(SystemConfig->KEY_SAVE))
		{
			Save();
		}
	}

	if (Keyboard::isKeyPressed(SystemConfig->KEY_NEWFILE))
	{
		NewLevel();
	}

	if (Keyboard::isKeyPressed(SystemConfig->KEY_GENLEVEL))
	{
		Init_Perlin_Noisy();

		if (toolW1 != nullptr)
			toolW1->texture = tex0;

		GenerateLevel();
	}

	if (Keyboard::isKeyPressed(SystemConfig->KEY_GENZPVE))
	{
		if (toolW1 != nullptr)
			toolW1->texture = tex0;

		zNoiseAxisValue++;
		GenerateLevel();
	}
	if (Keyboard::isKeyPressed(SystemConfig->KEY_GENZNEG))
	{
		if (toolW1 != nullptr)
			toolW1->texture = tex0;

		zNoiseAxisValue--;
		GenerateLevel();
	}

	if (Keyboard::isKeyPressed(Keyboard::P))
	{
		// Restart the application to play the scene.
		//TODO: Need to notify the player of any changes that need to be saved.
		SystemConfig->RestartApplication = true;
		SystemConfig->Options["editor"] = false;
		QCoreApplication::quit();
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

	// MOUSE INPUT
	mousePosition = Mouse::getPosition(*this);

	// map pixel position to gameView world space coordinates
	sf::Vector2f worldPos = this->mapPixelToCoords(mousePosition, *gameView);

	cursorPos = Vector2f(worldPos.x, worldPos.y);

	// Update LevelEditor
	MouseMoved(cursorPos, worldPos);
}

// Update the Camera's position and orientation.
void EditorApplication::UpdateCamera()
{
	// CAMERA MOVEMENT
	Vector2f direction = Vector2f(0, 0);

	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		direction.x = -(1 / zoomLevel);
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		direction.x = 1 / zoomLevel;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		direction.y = -(1 / zoomLevel);
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		direction.y = 1 / zoomLevel;
	}

	if (direction.x != 0 || direction.y != 0)
	{
		// MOVE CAMERA
		Vector2f cameraPos = Vector2f(direction.x * SystemConfig->fCameraMoveSpeed, direction.y * SystemConfig->fCameraMoveSpeed);

		gameView->move(cameraPos);

		Vector2i mousePosition;
		sf::Vector2f worldPos;

		mousePosition = Mouse::getPosition();
		mousePosition.x += direction.x;
		mousePosition.y += direction.y;

		// map pixel position to gameView world space coordinates.
		worldPos = this->mapPixelToCoords(mousePosition, *gameView);

		Mouse::setPosition(mousePosition);
	}
}

// Update the grid cells given current mouse state.
void EditorApplication::updateGrid()
{
	bool paintMode = toolW2->mode == 0;

	// given cellPos find the sprite and change the texture

	if (paintMode
		&& !hud->OwnsClickEvents // hack
		)
	{
		if ((cellPos.x >= 0 && cellPos.x <= this->world->cellCount) && (cellPos.y >= 0 && cellPos.y <= this->world->rows)) // bug?
		{
			if (spriteIndex >= 0 && spriteIndex < (this->world->columns * this->world->rows))
			{
				Sprite* cell = (*this->world->cells)[spriteIndex];

				if (toolW1->texture != nullptr)
				{
					cell->setTexture(*toolW1->texture);
				}
			}
		}

	}
}


// Renders the Editor's UI and Widgets.
void EditorApplication::RenderEditorUI(sf::RenderWindow* window, float deltaTime)
{
	Vector2u size = window->getSize();
	Vector2u tsize;
	Vector2f scale;

	// Widget 1, Texture selector tool
	tsize = toolW1->texture->getSize();
	scale = toolW1->overlay->getScale();

	if (toolW1->texture != nullptr)
	{
		toolW1->overlay->setTexture(*toolW1->texture);
	}

	toolW1->position = Vector2f(size.x - (tsize.x * scale.x), 0);
	toolW1->Render(window, deltaTime);


	// Widget 2, Paint/Line tool
	tsize = toolW2->texture->getSize();
	scale = toolW2->overlay->getScale();

	if (toolW2->texture != nullptr)
	{
		toolW2->overlay->setTexture(*toolW2->texture);
	}

	toolW2->position = Vector2f(size.x - (tsize.x * scale.x) - 200, 0);
	toolW2->Render(window, deltaTime);

	if (SystemConfig->IsEnabled(GameStartupOption::DEBUG_NOISE))
	{
		sf::Sprite overlayNoise;

		// Debug perlin noise texture
		if (HasNoiseChanged())
		{
			Uint8* pixels = GetNoisePixelData();

			sf::Texture textureNoise;
			textureNoise.create(NOISE_W, NOISE_H);
			textureNoise.update(pixels);

			overlayNoise.setPosition(200, 50);
			overlayNoise.setTexture(textureNoise);
		}

		window->draw(overlayNoise);
	}
}


// Loads the Editor's UI and Widgets.
void EditorApplication::LoadEditorUI()
{
	// Instantiate widgets
	widgets = new vector<Widget*>();
	toolW1 = new TextureToolWidget(this, textureManager->GetTextureByName(TEX_GRASS));
	toolW2 = new DrawToolWidget(this, textureManager->GetTextureByName(TEX_PAINTT));

	// EVENT SYSTEM setup
	widgets->push_back(toolW1);
	widgets->push_back(toolW2);

	// For now use this ui:
	hud = new HUD();

	EditorContext = this;

	hud->OnNewLevel = []() -> void {
		EditorContext->NewLevel();
	};

	hud->OnOpenLevel = [](string file) -> void {

		EditorContext->Load(file);
	};

	hud->OnSaveLevel = [](string file) -> void {

		EditorContext->Save(file);
	};

	hud->OnContextMenuCommand = [](string item, string id) -> void
	{
		Vector2i mousePosition = Mouse::getPosition(*(sf::RenderWindow*)EditorContext);

		// map pixel position to gameView world space coordinates
		sf::Vector2f worldPos = EditorContext->mapPixelToCoords(mousePosition, *EditorContext->gameView);

		if (id == "Camera") // Place Camera Here
		{

			EditorContext->MoveCamera(worldPos);

		}
		else if (id == "DeleteObject")
		{
			EditorApplication* editor = (EditorApplication*)EditorContext;

			GameObject* so = (GameObject*)editor->hud->selectedObject;
			if (so != nullptr)
			{
				if (editor->world->objectInstances != nullptr)
				{
					int index = std::find(editor->world->objectInstances->begin(), editor->world->objectInstances->end(), so) - editor->world->objectInstances->begin();
					editor->world->objectInstances->erase(editor->world->objectInstances->begin() + index);


					delete editor->hud->selectedObject;
					editor->hud->selectedObject = nullptr;
				}
			}

		}
		else
		{
			// Is a prefab to place

			((EditorApplication*)EditorContext)->PlacePrefab(id, worldPos);
		}
	};

	hud->RegisterWindow(EditorContext);
}

// Places a prefab at the specified position.
void EditorApplication::PlacePrefab(string name, Vector2f worldPosition)
{
	GameObject* inst;

	GameObject* objPrefab = (*world->gameObjectTypes)[name];

	if (world->objectInstances == nullptr)
	{
		world->objectInstances = new vector<GameObject*>();
	}

	// Create instance of prefab.
	inst = objPrefab->Instantiate(); // clone prefab

									 // Configure new instance with runtime properties.
	inst->transform = Transform::Identity;
	inst->transform.translate(worldPosition);
	inst->transform.rotate(0);

	inst->shape->setPosition(worldPosition);
	inst->shape->setRotation(0);

	// Add to scene.
	world->objectInstances->push_back(inst);
}

// Move the camera to the specified world position.
void EditorApplication::MoveCamera(Vector2f position)
{
	if (cameraSprite == nullptr)
	{
		// Show camera test object.
		sf::Texture* cameraTexture = textureManager->GetTextureByName(TEX_CAMERA);
		cameraSprite = new Sprite();
		cameraSprite->setTexture(*cameraTexture);
		Vector2u size = cameraTexture->getSize();
		cameraSprite->setOrigin(size.x / 2, size.y / 2);
	}

	cameraSprite->setPosition(position);
	world->cameraPosition = Vector3f(position.x, position.y, 0);
}

// Callback MouseReleased event from the owning window.
void EditorApplication::MouseReleased(Mouse::Button button, Vector2f mousePos, Vector2f worldPos)
{
	if (button == Mouse::Button::Left) {
		//HACK: hardcoded to left mouse button
		mousePosOnReleased = worldPos;
		mouseReleased = true;

		bool lineMode;
		Sprite* cell;

		lineMode = toolW2->mode > 0;

		if (lineMode && IsWithinGrid(mousePos))
		{
			Vector2i mousePosOnPressedI = TransformToGridSpace(SystemConfig->gridPosition, SystemConfig->cellOffset, mousePosOnPressed);
			Vector2i mousePosOnReleasedI = TransformToGridSpace(SystemConfig->gridPosition, SystemConfig->cellOffset, mousePosOnReleased);

			// FIND line points.
			vector<Vector2i> linePoints = Line(mousePosOnPressedI, mousePosOnReleasedI);

			for (int i = 0; i < linePoints.size(); i++)
			{
				Vector2i point = linePoints[i];

				// given cellPos find index into grid.
				spriteIndex = GetMatrixIndex(point.x, point.y, this->world->columns);

				if (spriteIndex >= 0 && spriteIndex < (this->world->cellCount))
				{
					cell = (*this->world->cells)[spriteIndex];

					if (toolW1->texture != nullptr)
					{
						// PAINT
						cell->setTexture(*toolW1->texture);
					}
				}
			}
		}
	}

	// PASS to EVENT SYSTEM
	Editor::MouseEvent* eventData = new Editor::MouseEvent();
	eventData->type = Editor::EventType::MouseReleased;
	eventData->button = button;
	eventData->worldPos2f = worldPos;
	eventData->position2f = mousePos;
	eventData->position2i = Vector2i(mousePos.x, mousePos.y);

	propagateEvent(eventData);
}



// Callback KeyPressed event from the owning window.
void EditorApplication::KeyPressed(Uint32 unicodeChar, char chr)
{
	// PASS to EVENT SYSTEM
	Editor::KeyEvent* eventData = new Editor::KeyEvent();

	eventData->type = Editor::EventType::KeyPressed;
	eventData->value = unicodeChar;
	eventData->key = chr;

	propagateEvent(eventData);
}

// Callback MouseMoved event from the owning window.
void EditorApplication::MouseMoved(Vector2f mousePosition, Vector2f worldPos)
{
	Vector2f cellOffset = SystemConfig->GetVector2("world", "v2fcelloffset");
	Vector2f gridPosition = SystemConfig->GetVector2("world", "v2fgridposition");

	mouseDelta = mousePosition - prevMousePos;

	// PASS to EVENT SYSTEM
	Editor::MouseEvent* eventData = new Editor::MouseEvent();
	eventData->type = Editor::EventType::MouseMoved;
	eventData->worldPos2f = worldPos;
	eventData->position2f = mousePosition;
	eventData->position2i = Vector2i(mousePosition.x, mousePosition.y);

	propagateEvent(eventData);


	// Is the mouse cursor within the grid?
	if (IsWithinGrid(mousePosition))
	{
		// transform mouse position to grid-space
		cellPos = TransformToGridSpace(gridPosition, cellOffset, mousePosition);

		// given cellPos find index into grid
		spriteIndex = GetMatrixIndex(cellPos.x, cellPos.y, this->world->columns);

		if (Mouse::isButtonPressed(Mouse::Button::Left)) //HACK: hardcoded to left mouse button
		{
			updateGrid();
		}
	}

	prevMousePos = mousePosition;
}

// Callback MousePressed event from the owning window.
void EditorApplication::MousePressed(Mouse::Button button, Vector2f mousePos, Vector2f worldPos)
{
	if (button == Mouse::Button::Left)
	{
		mousePosOnPressed = worldPos;
		mouseReleased = false;

		//TODO: optimise unrequired call to GetNearestComponent()
		Widget* nearest = GetNearestComponent(mousePos);

		if (nearest != nullptr)
		{
			if (this->focusedControl != nearest)
			{
				nearest->isFocused = true;

				setFocused(nearest);
			}
		}
		else
		{
			if (this->focusedControl != nullptr)
			{
				this->focusedControl->isFocused = false;

				this->focusedControl = nullptr;
			}
		}
	}

	if (button == Mouse::Button::Right)
	{
		// Is mouse within the bounds of the object nearest to it?

		GameObject* nearest = GetNearestObject(worldPos);

		if (nearest != nullptr)
		{
			SelectObject(nearest);
		}
	}

	// PASS to EVENT SYSTEM
	Editor::MouseEvent* eventData = new Editor::MouseEvent();
	eventData->type = Editor::EventType::MousePressed;
	eventData->button = button;
	eventData->worldPos2f = worldPos;
	eventData->position2f = mousePos;
	eventData->position2i = Vector2i(mousePos.x, mousePos.y);

	propagateEvent(eventData);
}

// Select the specified GameObject to display a context menu.
void EditorApplication::SelectObject(GameObject* obj) const
{
	hud->selectedObject = obj;

	cout << "clicked: a " << obj->name << endl;

	//OpenContextMenu(mousePosOnPressed);
}

// Propagates an event through the routes in the event system.
void EditorApplication::propagateEvent(Editor::Event* event)
{

	// Find widget in focus and send the event there
	Widget* w = GetComponentInFocus();

	if (w != nullptr)
	{
		w->HandleEvent(event);
	}

	delete event;
}

// Get the components proximal the specified point.
vector<Widget*> EditorApplication::GetNearestComponents(Vector2f point)
{
	vector<Widget*> nearestList;
	Widget* w;
	Widget* nearest = nullptr;
	int i;
	float distNearest = FLT_MAX;
	float dist;
	Rect<float> bbox;

	//TODO: optimise using space partitioning
	for (i = 0; i < widgets->size(); i++)
	{
		w = (*widgets)[i];

		dist = GetDistance(w->position, point);

		if (dist < distNearest)
		{
			bbox = w->GetBounds();

			if (WithinBounds(point, bbox))
			{
				distNearest = dist;
				nearest = w;

				nearestList.push_back(nearest);
			}
		}
	}

	return nearestList;
}

// Get the first GameObject proximal the specified point where the point is within bounds of the shape.
GameObject* EditorApplication::GetNearestObject(Vector2f point) // TODO: make this a generic function
{
	if (world->objectInstances == nullptr) return nullptr;

	GameObject* o;
	GameObject* nearest = nullptr;
	int i;
	float distNearest = FLT_MAX;
	float dist;
	bool foundNearest = false;
	FloatRect bbox;

	//TODO: optimise using space partitioning kd-tree or bsp-tree
	for (i = 0; i < world->objectInstances->size() && !foundNearest; i++)
	{
		o = (*world->objectInstances)[i];

		dist = GetDistance(o->shape->getPosition(), point);

		if (dist < distNearest)
		{
			bbox = o->shape->getGlobalBounds();

			if (WithinBounds(point, bbox))
			{
				distNearest = dist;
				nearest = o;

				foundNearest = true;
			}
		}
	}

	return nearest;
}

// Get the first component proximal the specified point where the point is within bounds of the control.
Widget* EditorApplication::GetNearestComponent(Vector2f point)
{
	Widget* w;
	Widget* nearest = nullptr;
	int i;
	float distNearest = FLT_MAX;
	float dist;
	bool foundNearest = false;
	Rect<float> bbox;

	//TODO: optimise using space partitioning kd-tree or bsp-tree
	for (i = 0; i < widgets->size() && !foundNearest; i++)
	{
		w = (*widgets)[i];

		dist = GetDistance(w->position, point);

		if (dist < distNearest)
		{
			bbox = w->GetBounds();

			if (WithinBounds(point, bbox))
			{
				distNearest = dist;
				nearest = w;

				foundNearest = true;
			}
		}
	}

	return nearest;
}




// Get the control currently in focus.
Widget* EditorApplication::GetComponentInFocus()
{
	return this->focusedControl;
}

// Set the specified component as the 'only one' that is focused at this time in the editor.
void EditorApplication::setFocused(Widget* component)
{
	this->focusedControl = component;
}

// Exit the application. Final state changes, signaling, and cleanup before application terminates.
void EditorApplication::Exit()
{
	if (!quittingApplication)
	{
		//this->close();

		quittingApplication = true;
	}
}

// Post state changes and signaling, issue final cleanup of object memory when application quits. 
EditorApplication::~EditorApplication()
{
	if (cursorEnabled)
	{
		this->setMouseCursorVisible(true);
	}

	delete levelProvider;
	delete widgets;
	delete toolW1;
	delete toolW2;
}