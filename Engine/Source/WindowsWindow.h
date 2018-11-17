#include "EngineBase.h"
#include "Window.h"
#include <windows.h>

class FWindowsWindow : public IWindow
{
public:
	FWindowsWindow(HINSTANCE HInstance, int Width, int Height);
	virtual void Tick();
	virtual void Show();

	//static const wchar_t AppWindowClass[];

private:
	HWND							Hwnd;
	std::shared_ptr<class FRHIViewport>	Viewport;
};