#include "Widget.h"
#include "MathHelpers.h"
#include "NoiseHelpers.h"

Widget::Widget(void* editor)
{
	this->levelEditor = editor;
	this->overlay = new Sprite();
	this->overlay->setScale(overlayScale);
}

Widget::Widget(void* editor, Texture* defaultTexture) : Widget(editor)
{
	this->levelEditor = editor;
	this->texture = defaultTexture;
	this->overlay->setTexture(*texture);
}

/*
Description: Renders the widget.
*/
void Widget::Render(RenderWindow* window, float deltaTime)
{
	if (isVisible)
	{
		Transform t;
		RenderStates states;

		t.translate(position);

		states.transform = t;

		window->draw(*overlay, states);
	}

	OnRender(window, deltaTime);
}

/*
Description: Raises a click event handler.
*/
void Widget::Click(Vector2f cursorPos)
{
	if (onClickHandler != nullptr)
	{
		onClickHandler(this, cursorPos);
	}
}

/*
Description: Callback MousePressed event from the owning window.
*/
void Widget::MousePressed(Mouse::Button button, Vector2f mousePos)
{
	if (button == Mouse::Button::Left)
	{
		Vector2u size = texture->getSize();
		Vector2f scale = overlay->getScale();

		Rect<float> bbox;
		bbox.left = position.x;
		bbox.top = position.y;
		bbox.width = size.x * scale.x;
		bbox.height = size.y * scale.y;

		if (WithinBounds(mousePos, bbox))
		{
			Click(mousePos);
		}
	}
}

/*
Description: Callback MouseReleased event from the owning window.
*/
void Widget::MouseReleased(Mouse::Button button, Vector2f mousePos)
{

}

/*
Description: Callback MouseMoved event from the owning window.
*/
void Widget::MouseMoved(Vector2f mousePosition)
{
	Vector2u size = texture->getSize();
	Vector2f scale = overlay->getScale();

	Rect<float> bbox;
	bbox.left = position.x;
	bbox.top = position.y;
	bbox.width = size.x * scale.x;
	bbox.height = size.y * scale.y;

	if (WithinBounds(mousePosition, bbox))
	{
		if (!isOver)
		{
			isOver = true;

			MouseOver(mousePosition);
		}
	}
	else
	{
		if (isOver)
		{
			MouseOut(mousePosition);

			isOver = false;
		}

	}
}

/*
Description: Gets the 2D bounding box of the current widget.
*/
Rect<float> Widget::GetBounds()
{
	Rect<float> bbox;
	Vector2f scale;
	Vector2u size;
	const Texture* tex;

	tex = overlay->getTexture();
	scale = overlay->getScale();
	size = tex->getSize();

	bbox.left = position.x;
	bbox.top = position.y;
	bbox.width = scale.x * size.x;
	bbox.height = scale.x * size.y;

	return bbox;
}

/*
Description: Handles an event propagated by the event system passing it to appropriate member functions.
*/
void Widget::HandleEvent(Editor::Event* event)
{
	Editor::KeyEvent* kevt;
	Editor::MouseEvent* mevt;

	// Pass event to appropriate member functions

	switch (event->type)
	{
	case Editor::EventType::KeyPressed:

		kevt = (Editor::KeyEvent*)event;

		OnKeyPressed(kevt->value, kevt->key);

		break;
	case Editor::EventType::MouseMoved:

		mevt = (Editor::MouseEvent*)event;

		MouseMoved(mevt->position2f);

		break;
	case Editor::EventType::MousePressed:

		mevt = (Editor::MouseEvent*)event;

		MousePressed(mevt->button, mevt->position2f);

		break;
	case Editor::EventType::MouseReleased:

		mevt = (Editor::MouseEvent*)event;

		MouseReleased(mevt->button, mevt->position2f);

		break;
	default:
		//cout << "unhandled event" << endl;
		break;
	}

	// TODO: Later broadcast to event listeners 
}

Widget::~Widget()
{
	delete overlay;
}
