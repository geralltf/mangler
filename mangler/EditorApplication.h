#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

#include <SFML/Graphics.hpp>
#include "GameSystem.h"
#include "Widget.h"
#include "EngineApplication.h"
#include "TextureManager.h"
#include "Level.h"
#include "HUD.h"

class EditorApplication : public EngineApplication
{
private:
	Widget											*toolW1, *toolW2;
	vector<Widget*>									*widgets;
	Widget											*focusedControl = nullptr;

	Vector2f										mousePosOnPressed;
	Vector2f										mousePosOnReleased;
	bool											mouseReleased;
	Vector2f										prevMousePos = Vector2f(0, 0);
	Vector2f										mouseDelta;
	Vector2i										cellPos;
	int												spriteIndex = -1;
	Sprite											*cameraSprite = nullptr;
public:HUD*											hud;
public:

	EditorApplication(GameConfig& config, MainFrame* Parent, const QPoint& Position, const QSize& Size);
	~EditorApplication();

	void OnInit() override;
	void OnUpdate() override;

	// Show a save level dialog.
	void Save();

	// Show a load level dialog.
	void Load();

	// Save the current level to file.
	void Save(string file) override;

	// Load the current level from file.
	void Load(string file) override;

	// Poll window events, then propagate to game and engine objects.
	void PollEvents() override;

	// Poll window keyboard and mouse input.
	void PollInput() override;

	// Update the Camera's position and orientation.
	void UpdateCamera() override;

	// Renders the Editor's UI and Widgets.
	void RenderEditorUI(sf::RenderWindow* window, float deltaTime);

	// Loads the Editor's UI and Widgets.
	void LoadEditorUI();

	// Exit the application. Final state changes, signaling, and cleanup before application terminates.
	void Exit() override;

protected:

	// EVENTS

	// Callback MouseReleased event from the owning window.
	void MouseReleased(Mouse::Button button, Vector2f mousePos, Vector2f worldPos);

	// Callback KeyPressed event from the owning window.
	void KeyPressed(Uint32 unicodeChar, char chr);

	// Callback MouseMoved event from the owning window.
	void MouseMoved(Vector2f mousePosition, Vector2f worldPos);

	// Callback MousePressed event from the owning window.
	void MousePressed(Mouse::Button button, Vector2f mousePos, Vector2f worldPos);

	// Propagates an event through the routes in the event system.
	void propagateEvent(Editor::Event* event);

	// Draw the scene.
	void DrawScene() override;

	// Move the camera to the specified world position.
	void MoveCamera(Vector2f position) override;

	// Places a prefab at the specified position.
	void PlacePrefab(string name, Vector2f worldPosition);

private:
	// Update the grid cells given current mouse state.
	void updateGrid();

	// Select the specified GameObject to display a context menu.
	void SelectObject(GameObject* obj) const;

	// Get the first GameObject proximal the specified point where the point is within bounds of the shape.
	GameObject* EditorApplication::GetNearestObject(Vector2f point);

	// Set the specified component as the 'only one' that is focused at this time in the editor.
	void setFocused(Widget* component);

	// Get the control currently in focus.
	Widget* GetComponentInFocus();

	// Get the components proximal the specified point.
	vector<Widget*> GetNearestComponents(Vector2f point);

	// Get the first component proximal the specified point where the point is within bounds of the control.
	Widget* GetNearestComponent(Vector2f point);
};

#endif