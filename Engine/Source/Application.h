#pragma once

#include "EngineBase.h"
#include <windows.h>
#include "Window.h"

class FApplication
{
public:
	FApplication(HINSTANCE HIstance);
	std::shared_ptr<IWindow>	AddWindow(int width, int height);
	static LRESULT CALLBACK		WindowProc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam);

private:
	HINSTANCE		HInstance;
	std::vector<std::shared_ptr<IWindow>>	Windows;
};