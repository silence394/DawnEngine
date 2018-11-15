#include <windows.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "d3d11.h"
#include <windowsx.h>
#include <tchar.h>

using namespace std;

const wchar_t* AppName = L"DawnEngine";

#pragma comment(lib, "dxgi.lib")

std::vector<HWND>	GWindows;

IDXGIFactory1*			GDXGIFactory = nullptr;
ID3D11Device*			GRenderDevice = nullptr;
ID3D11DeviceContext*	gDeviceContext = nullptr;

bool InitRenderDevice()
{
	if (GRenderDevice)
		return true;

	int flags = 0;

	#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&GDXGIFactory);

	HRESULT HResult = S_OK;
	IDXGIAdapter* Adapter;
	HResult = GDXGIFactory->EnumAdapters(0, &Adapter);

	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED(Adapter->GetDesc(&AdapterDesc)))
		return false;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL ActualFeatureLevel = (D3D_FEATURE_LEVEL)0;
	D3D11CreateDevice(
		Adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		flags,
		&FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&GRenderDevice,
		&ActualFeatureLevel,
		&gDeviceContext
	);

	if (FeatureLevel != ActualFeatureLevel)
		return false;

	return true;
}

class FViewport
{
public:
	FViewport(HWND Hwnd);

	ID3D11RenderTargetView*	GetBackBufferView() const
	{
		return BackSRV;
	}

	void	Present();

private:
	IDXGISwapChain*			SwapChain;
	ID3D11Texture2D*		BackBuffer;
	ID3D11RenderTargetView*	BackSRV;
};

void FViewport::Present()
{
	SwapChain->Present(0, 0);
}

FViewport::FViewport(HWND Hwnd)
{
	RECT rect;
	::GetClientRect(Hwnd, &rect);
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rect.right - rect.left;
	sd.BufferDesc.Height = rect.bottom - rect.top;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = Hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	if (GDXGIFactory->CreateSwapChain(GRenderDevice, &sd, (IDXGISwapChain**)&SwapChain) != 0)
		return;

	if (SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer) != 0)
		return;

	if (GRenderDevice->CreateRenderTargetView(BackBuffer, nullptr, &BackSRV) != 0)
		return;
}

class FWindow
{
public:
	FWindow(HINSTANCE HInstance, int Width, int Height);

	void	ShowWindow();

	void	Draw();

private:
	HWND					Hwnd;

	shared_ptr<FViewport>	Viewport;
};

FWindow::FWindow(HINSTANCE HInstance, int Width, int Height)
{
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exstyle = 0;
	Hwnd = CreateWindowEx(exstyle, AppName, AppName, style, 0, 0, Width, Height, NULL, NULL, HInstance, NULL);
}

void FWindow::ShowWindow()
{
	::ShowWindow(Hwnd, SW_SHOWNORMAL);

	if (Viewport == nullptr)
		Viewport = make_shared<FViewport>(Hwnd);
}

void FWindow::Draw()
{
	if (Viewport == nullptr)
		return;

	float color[4] = { 0.3f, 0.3f, 1.0f, 1.0f };
	gDeviceContext->ClearRenderTargetView(Viewport->GetBackBufferView(), color);

	Viewport->Present();
}

class FWindowApplication
{
public:
	static void					CreateWindowsApplication(HINSTANCE hInstance);
	static FWindowApplication&	Get();

	shared_ptr<FWindow>			AddWindow(int width, int height);

	void						Tick();

private:
	static LRESULT CALLBACK	WindowProc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam);

private:
	static HINSTANCE						HInstance;
	std::vector<std::shared_ptr<FWindow>>	Windows;
};

LRESULT CALLBACK FWindowApplication::WindowProc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	if (Msg == WM_CLOSE)
	{
		PostQuitMessage(0);
		GWindows.erase(find(GWindows.begin(), GWindows.end(), Hwnd));
	}

	return DefWindowProc(Hwnd, Msg, WParam, LParam);
}

void FWindowApplication::Tick()
{
	std::vector<std::shared_ptr<FWindow>>::iterator it = Windows.begin();
	while (it != Windows.end())
	{
		(*it)->Draw();
		it++;
	}
}

HINSTANCE FWindowApplication::HInstance = nullptr;

void FWindowApplication::CreateWindowsApplication(HINSTANCE HInstance)
{
	if (FWindowApplication::HInstance)
		return;

	FWindowApplication::HInstance = HInstance;

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
		return;
}

FWindowApplication& FWindowApplication::Get()
{
	static FWindowApplication windowapp;
	return windowapp;
}

shared_ptr<FWindow> FWindowApplication::AddWindow(int Width, int Height)
{
	shared_ptr<FWindow> WindowPtr(new FWindow(HInstance, Width, Height));
	Windows.push_back(WindowPtr);

	WindowPtr->ShowWindow();

	return WindowPtr;
}

int WINAPI WinMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd)
{
	// Preinit.
	InitRenderDevice();
	FWindowApplication::CreateWindowsApplication(hInInstance);
	FWindowApplication::Get().AddWindow(900, 600);

	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				if (GWindows.empty())
					break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			FWindowApplication::Get().Tick();

			Sleep(30);
		}
	}

	// TODO. ReleaseContext.
	system("pause");
}