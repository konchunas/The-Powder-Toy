#pragma once
#include "common/String.h"
#include "gui/interface/Point.h"
#include <vector>

class Graphics;
namespace ui
{

	enum ChromeStyle
	{
		None, Title, Resizable
	};

	class Engine;
	class Component;
	class Button;

	/* class Window
	 *
	 * A UI state. Contains all components.
	 */
	class Window
	{
	public:
		Point Position;
		Point Size;

		Window(Point _position, Point _size);
		virtual ~Window();

		void SetOkayButton(ui::Button * button) { okayButton = button; }
		void SetCancelButton(ui::Button * button) { cancelButton = button; }

		bool AllowExclusiveDrawing; //false will not call draw on objects outside of bounds
		bool DoesTextInput;

		// Add Component to window
		void AddComponent(Component* c);

		// Get the number of components this window has.
		unsigned GetComponentCount();

		// Get component by index. (See GetComponentCount())
		Component* GetComponent(unsigned idx);

		// Remove a component from window. NOTE: This DOES NOT free component from memory.
		void RemoveComponent(Component* c);

		// Remove a component from window. NOTE: This WILL free component from memory.
		void RemoveComponent(unsigned idx);

		virtual void ToolTip(ui::Point senderPosition, String toolTip) {}

		virtual void DoInitialized();
		virtual void DoExit();
		virtual void DoTick(float dt);
		virtual void DoDraw();
		virtual void DoFocus();
		virtual void DoBlur();
		virtual void DoFileDrop(ByteString filename);

		virtual void DoMouseMove(int x, int y, int dx, int dy);
		virtual void DoMouseDown(int x, int y, unsigned button);
		virtual void DoMouseUp(int x, int y, unsigned button);
		virtual void DoMouseWheel(int x, int y, int d);
		virtual void DoKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);
		virtual void DoKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);
		virtual void DoTextInput(String text);
		virtual void DoTextEditing(String text);
		virtual void DoGamepadButtonDown(int gamepad_id, int button);
		virtual void DoGamepadAxisMotion(int gamepad_id, int axis, int value);

		// Sets halt and destroy, this causes the Windows to stop sending events and remove itself.
		void SelfDestruct();
		void Halt();

		bool IsFocused(const Component* c) const;
		void FocusComponent(Component* c);

		void* UserData;

		enum OkayMethod { Enter, OkayButton };
		enum ExitMethod { MouseOutside, Escape, ExitButton };

		void MakeActiveWindow();
		bool CloseActiveWindow();
		Graphics * GetGraphics();

	protected:
		ui::Button * okayButton;
		ui::Button * cancelButton;

		virtual void OnInitialized() {}
		virtual void OnExit() {}
		virtual void OnTick(float dt) {}
		virtual void OnDraw() {}
		virtual void OnFocus() {}
		virtual void OnBlur() {}
		virtual void OnFileDrop(ByteString filename) {}

		virtual void OnTryExit(ExitMethod);
		virtual void OnTryOkay(OkayMethod);

		virtual void OnMouseMove(int x, int y, int dx, int dy) {}
		virtual void OnMouseDown(int x, int y, unsigned button) {}
		virtual void OnMouseUp(int x, int y, unsigned button) {}
		virtual void OnMouseWheel(int x, int y, int d) {}
		virtual void OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt) {}
		virtual void OnKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt) {}
		virtual void OnTextInput(String text) {}
		virtual void OnTextEditing(String text) {}
		virtual void OnGamepadButtonDown(int gamepad_id, int button) {}
		virtual void OnGamepadAxisMotion(int gamepad_id, int axis, int value) {}

		std::vector<Component*> Components;
		Component *focusedComponent_;
		Component *hoverComponent;
		ChromeStyle chrome;

		bool debugMode;
		//These controls allow a component to call the destruction of the Window inside an event (called by the Window)
		void finalise();
		bool halt;
		bool destruct;
		bool stop;

	};
}
