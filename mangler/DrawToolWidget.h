#ifndef WIDGET_DRAW_TOOL_H
#define WIDGET_DRAW_TOOL_H

#include "Widget.h"
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class DrawToolWidget : public Widget
{
public:

	DrawToolWidget(void* editor);
	DrawToolWidget(void* editor, Texture* defaultTexture);

	virtual void MouseOver(Vector2f mousePosition) override;
	virtual void MouseOut(Vector2f mousePosition) override;
	virtual void OnRender(RenderWindow* window, float deltaTime) override;
	virtual void OnKeyPressed(Uint32 unicodeChar, char chr) override;

	// Callback for Tool Line/Paint Widget OnClick event.
	void OnClick(void* o, Vector2f mousePosition);

	~DrawToolWidget();
};

#endif 