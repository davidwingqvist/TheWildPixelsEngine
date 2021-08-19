#pragma once
#include <Windows.h>
#include <Mouse.h>
#include <Keyboard.h>

namespace keyboard
{
	constexpr int KEY_A = 65;
	constexpr int KEY_B = 66;
	constexpr int KEY_C = 67;
	constexpr int KEY_D = 68;
	constexpr int KEY_E = 69;
	constexpr int KEY_F = 70;
	constexpr int KEY_G = 71;
	constexpr int KEY_H = 72;
	constexpr int KEY_I = 73;
	constexpr int KEY_J = 74;
	constexpr int KEY_K = 75;
	constexpr int KEY_L = 76;
	constexpr int KEY_M = 77;
	constexpr int KEY_N = 78;
	constexpr int KEY_O = 79;
	constexpr int KEY_P = 80;
	constexpr int KEY_Q = 81;
	constexpr int KEY_R = 82;
	constexpr int KEY_S = 83;
	constexpr int KEY_T = 84;
	constexpr int KEY_U = 85;
	constexpr int KEY_V = 86;
	constexpr int KEY_W = 87;
	constexpr int KEY_X = 88;
	constexpr int KEY_Y = 89;
	constexpr int KEY_Z = 90;
}

class InputHandler
{
private:

	std::unique_ptr<DirectX::Mouse> mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mouse_tracker;
	std::unique_ptr<DirectX::Keyboard> keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keyboard_tracker;

	DirectX::Mouse::State mouseState;
	DirectX::Keyboard::State keyboardState;
	MSG state;

	float x_mouse_pos = 0;
	float y_mouse_pos = 0;

	InputHandler();
	~InputHandler();

	bool shouldQuit = false;

public:

	static InputHandler* instance;
	static void Initialize();
	static void SetWindow(HWND& window);
	static void Destroy();
	static bool IsHeld(int keyCode);
	static bool IsPressedDown(int keyCode);

	static void Update();
	static bool HandleMessages();
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#define INPUT(key) InputHandler::instance->IsHeld(key)
#define PRESSED(key) InputHandler::instance->IsPressedDown(key)
#define UPDATE_INPUT InputHandler::instance->Update();

#define MOUSE_X InputHandler::instance->x_mouse_pos;
#define MOUSE_Y InputHandler::instance->y_mouse_pos;