#pragma once
#include <Windows.h>
class Window
{
private:

	HWND window;

public:

	Window();
	~Window();

	bool SetUp(HINSTANCE instance, UINT width, UINT height);

	HWND& getWindow();

};

