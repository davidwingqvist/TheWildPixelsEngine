#pragma once

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
public:

	InputHandler();
	~InputHandler();

	bool IsHeld(int keyCode);
	bool IsPressedDown(int keyCode);
};