#include "WindowsWindow.h"
#include "DynamicRHI.h"

//const wchar_t FWindowsWindow::AppWindowClass[] = TEXT("UnrealWindow");

FWindowsWindow::FWindowsWindow(HINSTANCE HInstance, int Width, int Height)
{
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exstyle = 0;
	Hwnd = CreateWindowEx(exstyle, L"DawnEngine", L"", style, 0, 0, Width, Height, NULL, NULL, HInstance, NULL);
}

void FWindowsWindow::Tick()
{

}

void FWindowsWindow::Show()
{
	::ShowWindow(Hwnd, SW_SHOWNORMAL);

	// Create d3d11 viewport default.
	if (Viewport == nullptr)
		Viewport = RHICreateViewport(Hwnd, false);
}