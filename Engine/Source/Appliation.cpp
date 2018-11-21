#include "Application.h"
#include "WindowsWindow.h"

LRESULT CALLBACK FApplication::WindowProc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	if (Msg == WM_CLOSE)
	{
		PostQuitMessage(0);
		// TODO. MultiWindows.
	}

	return DefWindowProc(Hwnd, Msg, WParam, LParam);
}

FApplication::FApplication(HINSTANCE InHIstance)
	: HInstance(InHIstance)
{
	// For Windows. TODO. Create a windows application class.
	WNDCLASS winclass;
	winclass.style = CS_BYTEALIGNCLIENT;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = HInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = TEXT("DawnEngine");

	if (!RegisterClass(&winclass))
		std::cout << "Register window class failed" << std::endl;
}

std::shared_ptr<IWindow> FApplication::AddWindow(int Width, int Height)
{
	IWindow* window = new FWindowsWindow(HInstance, Width, Height);
	std::shared_ptr<IWindow> WindowPtr(nullptr);
	Windows.push_back(WindowPtr);

	WindowPtr->Show();

	return WindowPtr;
}