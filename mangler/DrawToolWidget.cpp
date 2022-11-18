#include "DrawToolWidget.h"
#include "EditorApplication.h"
#include "Editor.h"

DrawToolWidget::DrawToolWidget(void* editor) : Widget(editor)
{
	// Default Callback
	this->onClickHandler = [](void* o, Vector2f mousePosition) -> void
	{
		DrawToolWidget* tool = (DrawToolWidget*)o;

		tool->OnClick(o, mousePosition);
	};

	mode = Editor::ToolType::PaintTool;
}

DrawToolWidget::DrawToolWidget(void* editor, Texture* defaultTexture) : Widget(editor, defaultTexture)
{
	// Default Callback
	this->onClickHandler = [](void* o, Vector2f mousePosition) -> void
	{
		DrawToolWidget* tool = (DrawToolWidget*)o;

		tool->OnClick(o, mousePosition);
	};

	mode = Editor::ToolType::PaintTool;
}

// Callback for Tool Line/Paint Widget OnClick event.
void DrawToolWidget::OnClick(void* o, Vector2f mousePosition)
{
	Widget* w = (Widget*)o;

	w->mode = (w->mode + 1) % Editor::ToolType::ToolCount;

	EditorApplication* editor = ((EditorApplication*)levelEditor);

	Texture* tex = editor->tex0;
	switch (w->mode)
	{
	case Editor::ToolType::PaintTool:
		tex = editor->textureManager->GetTextureByName(TEX_PAINTT);
		break;
	case Editor::ToolType::LineTool:
		tex = editor->textureManager->GetTextureByName(TEX_LINET);
		break;
	}
	w->overlay->setTexture(*tex);
	w->texture = tex;
}

void DrawToolWidget::MouseOver(Vector2f mousePosition)
{

}
void DrawToolWidget::MouseOut(Vector2f mousePosition)
{

}
void DrawToolWidget::OnRender(RenderWindow* window, float deltaTime)
{

}

void DrawToolWidget::OnKeyPressed(Uint32 unicodeChar, char chr)
{

}

DrawToolWidget::~DrawToolWidget()
{

}