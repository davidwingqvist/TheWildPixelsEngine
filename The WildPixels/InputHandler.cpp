#include "InputHandler.h"
#include "Graphics.h"
#include <dinput.h>
#include <iostream>
#include <DirectXMath.h>

InputHandler* InputHandler::instance = nullptr;

#define INSTANCE InputHandler::instance
using namespace DirectX;

InputHandler::InputHandler()
{
	mouse = std::make_unique<Mouse>();
	mouse_tracker = std::make_unique<Mouse::ButtonStateTracker>();
	keyboard = std::make_unique<Keyboard>();
	keyboard_tracker = std::make_unique<Keyboard::KeyboardStateTracker>();

}

InputHandler::~InputHandler()
{
}

float* InputHandler::GetMouseX()
{
	return &INSTANCE->x_mouse_pos;
}

float* InputHandler::GetMouseY()
{
	return &INSTANCE->y_mouse_pos;
}

void InputHandler::Initialize()
{
	if (!INSTANCE)
	{
		INSTANCE = new InputHandler();
		INSTANCE->x_mouse_pos = 0;
		INSTANCE->y_mouse_pos = 0;
	}
}

void InputHandler::SetWindow(HWND& window)
{
	INSTANCE->mouse->SetWindow(window);
	INSTANCE->mouse->SetMode(Mouse::Mode::MODE_ABSOLUTE);
}

void InputHandler::Destroy()
{
	if (INSTANCE)
		delete INSTANCE;
}

bool InputHandler::IsHeld(int keyCode)
{
	return GetAsyncKeyState(keyCode) < 0;
}

bool InputHandler::IsPressedDown(int keyCode)
{
	return (GetAsyncKeyState(keyCode) & WM_KEYUP) > 0;
}

void InputHandler::Update()
{
	INSTANCE->mouseState = INSTANCE->mouse->GetState();
	INSTANCE->mouse_tracker->Update(INSTANCE->mouseState);
	INSTANCE->keyboardState = INSTANCE->keyboard->GetState();
	INSTANCE->keyboard_tracker->Update(INSTANCE->keyboardState);

	// Convert the mouse coordinates to NDC space.

	// This converts the coordinates to min/max [0, 1]
	INSTANCE->x_mouse_pos = ((float)INSTANCE->mouse->GetState().x / (float)Graphics::GetWidth());
	INSTANCE->y_mouse_pos = ((float)INSTANCE->mouse->GetState().y / (float)Graphics::GetHeight());

	// Final convert to NDC [-1, 1] with [0, 0] being in the middle of the screen.
	// Y is positive upward and X is positive to the right.
	INSTANCE->x_mouse_pos = (INSTANCE->x_mouse_pos * 2.0f) - 1.0f;
	INSTANCE->y_mouse_pos = ((INSTANCE->y_mouse_pos * 2.0f) - 1.0f) * -1.0f;

	//std::cout << "X: " << INSTANCE->x_mouse_pos << " Y: " << INSTANCE->y_mouse_pos << "\n";
}

bool InputHandler::HandleMessages()
{
	while (PeekMessage(&InputHandler::instance->state, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&InputHandler::instance->state);
		DispatchMessage(&InputHandler::instance->state);
	}

	return INSTANCE->shouldQuit;
}

LRESULT InputHandler::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		DestroyWindow(hWnd);
		INSTANCE->shouldQuit = true;
	case WM_ACTIVATEAPP:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_INPUT:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_MOUSEMOVE:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_LBUTTONDOWN:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_LBUTTONUP:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_RBUTTONDOWN:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_RBUTTONUP:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_MBUTTONDOWN:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_MBUTTONUP:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_MOUSEWHEEL:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_XBUTTONDOWN:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_XBUTTONUP:
		Mouse::ProcessMessage(message, wParam, lParam);
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
