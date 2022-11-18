#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Event.h"

using namespace sf;

typedef void(*OnClickFunctPointer)(void* o, Vector2f mousePosition);

class Widget
{
public:
	Vector2f position;
	bool isVisible = true;
	bool isFocused = false;
	Sprite* overlay;
	Vector2f overlayScale = Vector2f(1.0f, 1.0f);
	Texture* texture;
	OnClickFunctPointer onClickHandler = nullptr;
	bool isOver = false;
	int mode = 2;
	void* levelEditor;

	Widget(void* editor);
	Widget(void* editor, Texture* defaultTexture);

	/*
	Description: Renders the widget.
	*/
	void Render(RenderWindow* window, float deltaTime);

	/*
	Description: Gets the 2D bounding box of the current widget.
	*/
	Rect<float> GetBounds();

	/*
	Description: Handles an event propagated by the event system passing it to appropriate member functions.
	*/
	void HandleEvent(Editor::Event* event);

	/*
	Description: Raises a click event handler.
	*/
	void Click(Vector2f cursorPos);

	/*
	Description: Callback MousePressed event from the owning window.
	*/
	void MousePressed(Mouse::Button button, Vector2f mousePos);

	/*
	Description: Callback MouseReleased event from the owning window.
	*/
	void MouseReleased(Mouse::Button button, Vector2f mousePos);

	/*
	Description: Callback MouseMoved event from the owning window.
	*/
	void MouseMoved(Vector2f mousePosition);

	virtual void MouseOver(Vector2f mousePosition) = 0;
	virtual void MouseOut(Vector2f mousePosition) = 0;
	virtual void OnRender(RenderWindow* window, float deltaTime) = 0;
	virtual void OnKeyPressed(Uint32 unicodeChar, char chr) = 0;

	~Widget();
};

#endif