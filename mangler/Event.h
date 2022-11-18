#ifndef EVENT_H
#define EVENT_H

#include <SFML/Graphics.hpp>

using namespace sf;

/* The Event System Model */

namespace Editor
{
	enum EventType
	{
		MouseMoved,
		MousePressed,
		MouseReleased,
		KeyPressed
	};

	class Event
	{
	public:
		// Data:
		//void* data;
		EventType type;
	};

	/* Event Types */

	class KeyEvent : public Event
	{
	public:
		// Data:
		Uint32 value; // unicode
		char key;
	};

	class MouseEvent : public Event
	{
	public:
		// Data:
		Mouse::Button button;
		Vector2f worldPos2f;
		Vector2f position2f;
		Vector2i position2i;
	};
}

#endif