#include "InputHandler.h"
#include <dinput.h>

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

bool InputHandler::IsHeld(int keyCode)
{
	return GetAsyncKeyState(keyCode) < 0;
}

bool InputHandler::IsPressedDown(int keyCode)
{
	return (GetAsyncKeyState(keyCode) & WM_KEYUP) > 0;
}
