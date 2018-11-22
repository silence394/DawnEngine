#include "EngineBase.h"
#include "Window.h"
#include <windows.h>

class FWindowsWindow : public IWindow
{
public:
	FWindowsWindow(HINSTANCE HInstance, int Width, int Height);
	virtual void Tick();
	virtual void Show();

	const static wchar_t*	ApplicatioName;

private:
	HWND							Hwnd;
	std::shared_ptr<class FRHIViewport>	Viewport;
};