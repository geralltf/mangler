#include "TextureToolWidget.h"
#include "EditorApplication.h"
#include "Editor.h"

TextureToolWidget::TextureToolWidget(void* editor) : Widget(editor)
{
	// Default Callback
	this->onClickHandler = [](void* o, Vector2f mousePosition) -> void
	{
		TextureToolWidget* tool = (TextureToolWidget*)o;

		tool->OnClick(o, mousePosition);
	};

	this->mode = Editor::TextureToolMode::TEX0;
}

TextureToolWidget::TextureToolWidget(void* editor, Texture* defaultTexture) : Widget(editor, defaultTexture)
{
	// Default Callback
	this->onClickHandler = [](void* o, Vector2f mousePosition) -> void
	{
		TextureToolWidget* tool = (TextureToolWidget*)o;

		tool->OnClick(o, mousePosition);
	};

	this->mode = Editor::TextureToolMode::TEX0;
}

// Callback for Tool Widget OnClick event.
void TextureToolWidget::OnClick(void* o, Vector2f mousePosition)
{

	EditorApplication* editor;
	Widget* w;
	Texture* tex;

	w = (Widget*)o;
	w->mode = (w->mode + 1) % Editor::TextureToolMode::TextureCount;

	editor = ((EditorApplication*)levelEditor);

	// SELECT TEXTURE
	tex = editor->tex0;
	switch (w->mode)
	{
	case Editor::TextureToolMode::TEX0:
		tex = editor->tex0;
		break;
	case Editor::TextureToolMode::TEX1:
		tex = editor->tex1;
		break;
	case Editor::TextureToolMode::TEX2:
		tex = editor->tex2;
		break;
	case Editor::TextureToolMode::TEX3:
		tex = editor->tex3;
		break;
	case Editor::TextureToolMode::TEX4:
		tex = editor->tex4;
		break;
	case Editor::TextureToolMode::TEX5:
		tex = editor->tex5;
		break;
	}
	w->overlay->setTexture(*tex);
	w->texture = tex;
}

void TextureToolWidget::MouseOver(Vector2f mousePosition)
{

}
void TextureToolWidget::MouseOut(Vector2f mousePosition)
{

}
void TextureToolWidget::OnRender(RenderWindow* window, float deltaTime)
{

}
void TextureToolWidget::OnKeyPressed(Uint32 unicodeChar, char chr)
{

}

TextureToolWidget::~TextureToolWidget()
{

}