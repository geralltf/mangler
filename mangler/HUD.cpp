#include "HUD.h"
#include <TGUI/TGUI.hpp>

#define SAVE_FILE "save.g.xml"

void HUD::rightClickCallback(tgui::Gui* _gui, sf::Vector2f position)
{
	HUD* self = this;
	map<string, void*>::iterator it;

	contextMenu = theme->load("ListBox");

	if (selectedObject != nullptr)
	{
		contextMenu->addItem("Delete Object", "DeleteObject");
	}


	contextMenu->addItem("Place Camera Here", "Camera");

	//if (app->world->gameObjectTypes != nullptr)
	//{
	//	for (it = app->world->gameObjectTypes->begin(); it != app->world->gameObjectTypes->end(); it++)
	//	{
	//		stringstream ss;
	//		ss << "Place New " << it->first;
	//		contextMenu->addItem(ss.str(), it->first);
	//	}
	//}

	contextMenu->addItem("Test hello world", "id_test");

	contextMenu->setItemHeight(20);
	contextMenu->setPosition(position);
	contextMenu->setSize(180, contextMenu->getItemHeight() * contextMenu->getItemCount() * 1.2);
	contextMenu->connect("ItemSelected", [self](string item)->void
	{
		//string id = contextMenu->getSelectedItemId();
		string id = self->contextMenu->getSelectedItemId();

		self->OnContextMenuCommand(item, id);
		//std::cout << item << std::endl;
	});
	gui->add(contextMenu);
}

HUD::HUD()
{
	Visible = true;
}

HUD::~HUD()
{
	delete gui;
}

void HUD::OpenLoad()
{
	Visible = true;
	modeOpen = true;
	button->setText("Open Level");
}

void HUD::OpenSave()
{
	Visible = true;
	modeOpen = false;
	button->setText("Save Level");
}

int HUD::RegisterWindow(RenderWindow* window)
{
	this->window = window;
	gui = new tgui::Gui(*window);
	guiGame = new tgui::Gui(*window);

	try
	{
		// Load the black theme
		theme = tgui::Theme::create("Assets\\widgets\\Black.txt");

		// Load the widgets
		LoadTitleScreen(*gui);
	}
	catch (const tgui::Exception& e)
	{
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

void HUD::LoadTitleScreen(tgui::Gui& gui)
{
	// Get a bound version of the window size
	// Passing this to setPosition or setSize will make the widget automatically update when the view of the gui changes
	auto windowWidth = tgui::bindWidth(gui);
	auto windowHeight = tgui::bindHeight(gui);

	// Create the background image (picture is of type tgui::Picture::Ptr or std::shared_widget<Picture>)
	auto picture = tgui::Picture::create("Assets\\titlescreen-background.png");
	picture->setSize(tgui::bindMax(800, windowWidth), tgui::bindMax(600, windowHeight));
	gui.add(picture);

	// Create the password edit box
	tgui::EditBox::Ptr editBoxFilename = theme->load("EditBox");
	editBoxFilename->setSize(windowWidth * 2 / 3, windowHeight / 8);
	editBoxFilename->setPosition(windowWidth / 6, windowHeight * 5 / 12);
	editBoxFilename->setDefaultText(SAVE_FILE);
	gui.add(editBoxFilename, "Filename");

	// Create the login button
	button = theme->load("Button");
	button->setSize(windowWidth / 2, windowHeight / 6);
	button->setPosition((windowWidth / 2.1), windowHeight * 7 / 10);
	button->setText("Open Level");
	gui.add(button);

	tgui::Button::Ptr button2 = theme->load("Button");
	button2->setSize(windowWidth / 4, windowHeight / 6);
	button2->setPosition((windowWidth / 10), windowHeight * 7 / 10);
	button2->setText("New");
	gui.add(button2);

	HUD* self = this;
	std::string saveFile = SAVE_FILE;

	// Call the OnOpenLevel callback function when the button is pressed
	button->connect("pressed", [self, saveFile](tgui::EditBox::Ptr filename) -> void
	{
		string fileName = filename->getText().toAnsiString();

		if (fileName == "")
			fileName = saveFile;

		self->Visible = false;

		if (self->modeOpen)
		{
			self->OnOpenLevel(fileName);
		}
		else
		{
			self->OnSaveLevel(fileName);
		}


	}, editBoxFilename);

	button2->connect("pressed", [self]() -> void
	{
		self->Visible = false;
		self->OnNewLevel();
	});
}

void HUD::PropagateEvent(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::Resized:
		gui->setView(window->getView());
		guiGame->setView(window->getView());
	default:
		gui->handleEvent(event);
		guiGame->handleEvent(event);
	}

	// Check if there is a pop-up menu
	if (contextMenu)
	{
		// When mouse is released, remove the pop-up menu
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			guiGame->remove(contextMenu);
			contextMenu = nullptr;

			OwnsClickEvents = false;
		}

		// When mouse is pressed, remove the pop-up menu only when the mouse is not on top of the menu
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (!contextMenu->mouseOnWidget(event.mouseButton.x, event.mouseButton.y))
			{
				guiGame->remove(contextMenu);
				contextMenu = nullptr;

				OwnsClickEvents = false;
			}
		}
	}

	// Perhaps we have to open a menu
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		rightClickCallback(guiGame, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));

		OwnsClickEvents = true; // make sure editor cant use the mouse until context menu does away
	}
}

void HUD::Render() const
{
	// Render all the TGUI Widgets

	if (Visible)
	{
		// Title screen
		gui->draw();
	}
	else
	{
		// In Game
		guiGame->draw();
	}
}