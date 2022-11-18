#include "EngineApplication.h"
#include "MathHelpers.h"
#include "NoiseHelpers.h"

EngineApplication::EngineApplication(GameConfig& config, MainFrame* Parent, const QPoint& Position, const QSize& Size)
	: RenderCanvas(config, Parent, Position, Size), SystemConfig((GameConfig*)&config)
{
	textureManager = new TextureManager(SystemConfig);
	IsGameFullscreen = false;
}

EngineApplication::EngineApplication(GameConfig& config, MainFrame* Parent) : RenderCanvas(config, Parent), SystemConfig((GameConfig*)&config)
{
	textureManager = new TextureManager(SystemConfig);
	IsGameFullscreen = true;
}

// Run the application render loop.
void EngineApplication::Run()
{
	if (IsGameFullscreen)
	{
		// Issue own render loop since we are now operating from standalone RenderWindow.
		OnInit();
		
		setIcon(32, 32, MakeGameIcon());
		setTitle(SystemConfig->GetAppTitle());

		while (isOpen() && !quittingApplication)
		{
			sf::Event e;
			while (pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					RenderWindow::close();
				}
				if (e.type == sf::Event::KeyPressed)
				{
					if (e.key.code == sf::Keyboard::Key::Escape)
					{
						RenderWindow::close();
					}
				}
			}

			// Render on screen
			OnUpdate();
			display();
		}
	}
}

void EngineApplication::OnInit()
{
	// CREATE viewports
	_width = getSize().x;
	_height = getSize().y;
	_aspectRatio = _width / _height;

	// Setup Viewports.
	uiView = new View(sf::FloatRect(0, 0, _width, _height / 2));
	gameView = new View(sf::FloatRect(0, 0, _width, _height));
	minimapView = new View(sf::FloatRect(0, 0, _width, _height));

	// the ui view (partial view).
	uiView->setViewport(sf::FloatRect(0, 0, 1, 1));
	uiView->setCenter(_width / 2, _height / 2);
	uiView->setSize(_width, _height);

	// the game view (full window).
	gameView->setViewport(sf::FloatRect(0, 0, 1, 1));
	gameView->setCenter(_width / 2, _height / 2);
	gameView->setSize(_width, _height);

	// mini-map (bottom right corner).
	minimapView->setViewport(sf::FloatRect(0.77f, 0.75f, 0.25, 0.25));
	minimapView->setSize(_width * 2, _height * 2);


	if (SystemConfig->IsEnabled(GameStartupOption::CAP_FPS))
	{
		float framerateFreq = SystemConfig->GetFloat("display", "fcapframeratefreg");

		setVerticalSyncEnabled(true);
		setFramerateLimit(framerateFreq);
	}
}

void EngineApplication::OnUpdate()
{

}

// Clears the view and state for a New Level.
void EngineApplication::NewLevel()
{
	Sprite* sprite;
	Vector2f pos;
	int r, c;
	Texture* defaultTexture;

	Vector2f cellOffset = SystemConfig->GetVector2("world", "v2fcelloffset");
	Vector2f cellScale = SystemConfig->GetVector2("world", "v2fcellscale");
	Vector2f gridPosition = SystemConfig->GetVector2("world", "v2fgridposition");
	Vector2f gridSize = SystemConfig->GetVector2("world", "v2fgridsize");

	this->world->columns = gridSize.x;
	this->world->rows = gridSize.y;
	this->world->cellCount = gridSize.x * gridSize.y;

	this->world->cells = new vector<Sprite*>();
	defaultTexture = tex2;

	// RECREATE DEFAULT GRID
	for (r = 0; r < this->world->rows; r++)
	{
		for (c = 0; c < this->world->columns; c++)
		{
			sprite = new Sprite();

			pos.x = c * cellOffset.x;
			pos.y = r * cellOffset.y;

			sprite->setScale(cellScale);
			sprite->setTexture(*defaultTexture);
			sprite->setPosition(gridPosition + pos);

			this->world->cells->push_back(sprite);
		}
	}

	// Clear left over game objects
	if (world->objectInstances != nullptr)
	{
		delete world->objectInstances;
		world->objectInstances = nullptr;
	}
	if (world->gameObjectTypes != nullptr)
	{
		delete world->gameObjectTypes;
		world->gameObjectTypes = nullptr;
	}
}

// Generates a random level using perlin noise.
void EngineApplication::GenerateLevel()
{
	Sprite* sprite;
	Vector2f pos;
	int r, c;
	Texture* texture;
	int tex;
	float n; // noise value

	Vector2f cellOffset = SystemConfig->GetVector2("world", "v2fcelloffset");
	Vector2f cellScale = SystemConfig->GetVector2("world", "v2fcellscale");
	Vector2f gridPosition = SystemConfig->GetVector2("world", "v2fgridposition");

	this->world->cells = new vector<Sprite*>();

	// Init_Perlin_Noisy();

	// RECREATE DEFAULT GRID
	for (r = 0; r < this->world->rows; r++)
	{
		for (c = 0; c < this->world->columns; c++)
		{
			sprite = new Sprite();

			pos.x = c * cellOffset.x;
			pos.y = r * cellOffset.y;

			sprite->setScale(cellScale);
			sprite->setPosition(gridPosition + pos);

			texture = tex4; // white


							// Compute noise value 
			n = PerlinNoise3D(c, r, zNoiseAxisValue);

			//cout << n << endl;

			// Threshold noise into different texture groups
			if (n < 0.1f) texture = tex2; // water
			else if (n < 0.15f) texture = tex0; // grass
			else if (n < 0.3f) texture = tex2; // water
			else if (n < 0.4f) texture = tex2; // water
			else if (n < 0.5f) texture = tex3; // mountain
			else if (n < 0.6f) texture = tex5; // black
			else if (n < 0.7f) texture = tex1; // fire
			else texture = tex4; // white
								 //TODO: research fussy thresholding

								 // Assign texture
			sprite->setTexture(*texture);


			this->world->cells->push_back(sprite);
		}
	}
}

// Load a blank Scene and the Asset folder.
void EngineApplication::LoadScene()
{
	Sprite* sprite;
	Texture* defaultTexture;
	Vector2f pos;
	int i, r, c;

	// SYSTEM ASSETS
	textureManager->LoadAllAssets();

	// Hacky
	tex0 = textureManager->GetTextureByName(TEX_GRASS);
	tex1 = textureManager->GetTextureByName(TEX_FIRE);
	tex2 = textureManager->GetTextureByName(TEX_BLACK);
	tex3 = textureManager->GetTextureByName(TEX_MOUNTAIN);
	tex4 = textureManager->GetTextureByName(TEX_WHITE);
	tex5 = textureManager->GetTextureByName(TEX_BLACK);
	texp1L = textureManager->GetTextureByName(TEX_PLAYER_L);
	texp1R = textureManager->GetTextureByName(TEX_PLAYER_R);

	world = new Level();

	// game:
	world->cells = new vector<Sprite*>();
	defaultTexture = tex2;

	//editor:
	NewLevel();
}

// Loads the System Font Assets.
void EngineApplication::LoadFonts()
{
	// FONTS
	if (!fontDefault.loadFromFile(SystemConfig->fontDefault))
	{
		stringstream ss;
		ss << SystemConfig->assetFolder << SystemConfig->PATH_SEPARATOR << SystemConfig->fontDefault;
		fontDefault.loadFromFile(ss.str());
	}
}

// Loads and configures the system cursor.
void EngineApplication::LoadGCursor()
{
	cursorTexture = textureManager->GetTextureByName(TEX_CURSOR);
	cursorTexture2 = textureManager->GetTextureByName(TEX_CURSOR2);

	cursorSprite = new Sprite();
	cursorSprite->setScale(SystemConfig->cursorScale);
	cursorSprite->setTexture(*cursorTexture);

	cursorSprite2 = new Sprite();
	cursorSprite2->setScale(SystemConfig->cursorScale);
	cursorSprite2->setTexture(*cursorTexture2);

	// Set the middle point of the cursor to be the origin.
	Vector2u size = cursorTexture->getSize();
	Vector2u size2 = cursorTexture2->getSize();
	cursorSprite->setOrigin(size.x / 2, size.y / 2);
	cursorSprite2->setOrigin(size2.x / 2, size2.y / 2);
}

void EngineApplication::RenderCursor(RenderWindow* window, Vector2f cursorPosition, float deltaTime)
{
	RenderStates states;
	Transform t;

	t.translate(cursorPosition);
	states.transform = t;

	if (titleScreen)
	{
		window->draw(*cursorSprite2, states);
	}
	else
	{
		window->draw(*cursorSprite, states);
	}
}


// Renders the current scene.
void EngineApplication::RenderScene(sf::RenderWindow* window, float deltaTime)
{
	sf::Sprite sprite;
	sf::RenderStates states;

	// RENDER GRID
	for (int i = 0; i < this->world->cells->size(); i++)
	{
		sprite = *(*this->world->cells)[i];

		window->draw(sprite, states);
	}
}

// Renders the current scene.
void EngineApplication::RenderObjects(RenderWindow* window, float deltaTime)
{
	if (this->world->objectInstances == nullptr) return;

	GameObject obj;
	sf::RenderStates states;

	// RENDER OBJECTS
	for (int i = 0; i < this->world->objectInstances->size(); i++)
	{
		obj = *(*this->world->objectInstances)[i];

		window->draw(*obj.shape, states);
	}
}

// Draw a scene for a view with the game system's cursor. 
void EngineApplication::DrawScene()
{
	RenderScene(this, deltaTime);

	if (cursorEnabled)
	{
		// Show Game Cursor.
		RenderCursor(this, cursorPos, deltaTime);

		// Hide System Cursor.
		this->setMouseCursorVisible(false);
	}
}

// Rotate the view in the pve + direction.
void EngineApplication::RotatePos()
{
	gameView->rotate(SystemConfig->rotationAnglePve);
}

// Rotate the view in the neg - direction.
void EngineApplication::RotateNeg()
{
	gameView->rotate(SystemConfig->rotationAngleNeg);
}

// Zoom the view in.
void EngineApplication::ZoomIn()
{
	gameView->zoom(SystemConfig->zoomInFactor);
}

// Zoom the view out.
void EngineApplication::ZoomOut()
{
	gameView->zoom(SystemConfig->zoomOutFactor);
}

// Window resized event; Update the aspect ratio of the window.
void EngineApplication::OnResize()
{
	// ASPECT RATIO

	// update the views to the new size of the window.
	_width = event.size.width;
	_height = event.size.height;
	_aspectRatio = _width / _height;

	// the ui view (partial view).
	uiView->setCenter(_width / 2, _height / 2);
	uiView->setSize(_width, _height);

	// the game view (full window).
	gameView->setCenter(_width / 2, _height / 2);
	gameView->setSize(_width, _height);

	// mini-map (bottom right corner).
	minimapView->setSize(_width * 2, _height * 2);
}

// Calculate if the mouse position lies within the grid.
bool EngineApplication::IsWithinGrid(Vector2f position)
{
	Rect<float> bbox; // grid bounding box

	Vector2f cellOffset = SystemConfig->GetVector2("world", "v2fcelloffset");
	Vector2f gridPosition = SystemConfig->GetVector2("world", "v2fgridposition");

	// MOUSE to grid-space
	bbox.left = gridPosition.x;
	bbox.top = gridPosition.y;
	bbox.width = this->world->columns * cellOffset.x;
	bbox.height = this->world->rows * cellOffset.y;

	return WithinBounds(position, bbox);
}

// Draws a line between the specified points.
void EngineApplication::DrawLine(Vector2f a, Vector2f b, Color colour)
{
	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = a;
	lines[0].color = colour;
	lines[1].position = b;
	lines[1].color = colour;
	this->draw(lines);

	// Line with thickness:
	//sf::RectangleShape line(sf::Vector2f(150, 5));
	//line.rotate(45);
	//window->draw(line);
}

// Makes a system icon to be used for the underlying window.
Uint8* EngineApplication::MakeGameIcon()
{
	Uint8* icon = nullptr;
	Texture* tex = new Texture();
	std::string folder = SystemConfig->Game["game path"]["sassetfolder"];
	std::string file = SystemConfig->Game["game path"]["sgameicon"];

	if (!tex->loadFromFile(file))
	{
		stringstream ss;
		ss << folder << SystemConfig->PATH_SEPARATOR << file;
		if (!tex->loadFromFile(ss.str()))
		{
			tex = nullptr;
		}
	}

	if (tex != nullptr)
	{
		Image* clone = new Image(tex->copyToImage());
		icon = (Uint8*)clone->getPixelsPtr();
		delete tex;
	}

	return icon;
}

EngineApplication::~EngineApplication()
{
	delete cursorTexture;
	delete textureManager;
}