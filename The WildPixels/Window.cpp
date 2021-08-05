#include "Window.h"
#include <WinUser.h>
#include <string>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window()
{
	this->window = {};
}

Window::~Window()
{
}

bool Window::SetUp(HINSTANCE instance, UINT width, UINT height)
{
	const wchar_t CLASS_NAME[] = L"Test Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

#ifdef _DEBUG
	window = CreateWindowEx(0, CLASS_NAME, L"The WildPixels Engine", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height, nullptr, nullptr, instance, nullptr);
#else
	window = CreateWindowEx(0, CLASS_NAME, L"GAME NAME", WS_POPUP,
		CW_USEDEFAULT, 0, width, height, nullptr, nullptr, instance, nullptr)
#endif

	if (window == nullptr)
	{
		return false;
	}

	ShowWindow(window, SW_SHOW);
	return true;
}

HWND& Window::getWindow()
{
	return this->window;
}
