#ifndef HUD_H
#define HUD_H

#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include  <TGUI/Loading/Theme.hpp>

using namespace std;
using namespace sf;

class HUD
{
public:
	typedef void(*OnOpenLevelFunct)(string fileName);
	typedef void(*OnNewLevelFunct)();
	typedef void(*OnContextMenuCommandFunct)(string item, string id);

private:
	RenderWindow* window;
	tgui::Gui *gui, *guiGame;
	tgui::Theme::Ptr theme;
	tgui::Button::Ptr button;
	tgui::ListBox::Ptr contextMenu;
public:
	void* selectedObject;
	bool modeOpen = true;
	bool Visible;
	bool OwnsClickEvents = false;
	OnOpenLevelFunct OnOpenLevel;
	OnOpenLevelFunct OnSaveLevel;
	OnNewLevelFunct OnNewLevel;
	OnContextMenuCommandFunct OnContextMenuCommand;
public:
	HUD();
	~HUD();

	void OpenSave();
	void OpenLoad();

	// Register GUI for the specified window.
	int RegisterWindow(RenderWindow* window);

	void PropagateEvent(sf::Event event);

	void Render() const;

private:
	// Load all the widgets 
	void LoadTitleScreen(tgui::Gui& _gui);
	void rightClickCallback(tgui::Gui* _gui, sf::Vector2f position);
};

#endif