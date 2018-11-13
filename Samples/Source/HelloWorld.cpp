#include <windows.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "d3d11.h"
#include <windowsx.h>
#include <tchar.h>

using namespace std;

const wchar_t AppName[] = TEXT("DawnEngine");

#pragma comment(lib, "dxgi.lib")
HWND	GNativeWindow;
HWND	GWindow2;

std::vector<HWND>	GWindows;

IDXGIFactory1*			GDXGIFactory = nullptr;
ID3D11Device*			GRenderDevice = nullptr;
ID3D11DeviceContext*	gDeviceContext;

IDXGISwapChain*			GNativeSwapChain;
IDXGISwapChain*			GSwapChain2;
ID3D11RenderTargetView*	GRTV1 = nullptr;
ID3D11RenderTargetView*	GRTV2 = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
		GWindows.erase(find(GWindows.begin(), GWindows.end(), hwnd));
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

HWND CreateMyWindow(int width, int height, LPCWSTR name)
{
	WNDCLASS winclass;

	winclass.style = CS_BYTEALIGNCLIENT;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = GetModuleHandle(NULL);
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = AppName;

	if (!RegisterClass(&winclass))
		return NULL;

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exstyle = 0;
	HWND hwnd = CreateWindowEx(exstyle, AppName, AppName, style, 0, 0, width, height, NULL, NULL, winclass.hInstance, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	return hwnd;
}

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

bool CreateBackBuffer(HWND window, IDXGISwapChain*& swapchain, ID3D11RenderTargetView*& rtv)
{
	// Create Swap Chain.
	RECT rect;
	::GetClientRect(window, &rect);
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rect.right - rect.left;
	sd.BufferDesc.Height = rect.bottom - rect.top;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = (HWND)window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	if (GDXGIFactory->CreateSwapChain(GRenderDevice, &sd, (IDXGISwapChain**)&swapchain) != 0)
		return false;

	ID3D11Texture2D* buffer = nullptr;
	if (swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer) != 0)
		return false;

	if (GRenderDevice->CreateRenderTargetView(buffer, nullptr, &rtv) != 0)
		return false;

	return true;
}

class FWindow
{
public:
	FWindow(HINSTANCE HInstance, int Width, int Height);

	void ShowWindow();

private:
	HWND	Hwnd;
};

FWindow::FWindow(HINSTANCE HInstance, int Width, int Height)
{
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exstyle = 0;
	Hwnd = CreateWindowEx(exstyle, TEXT("DawnEngine"), TEXT("DawnEngine"), style, 0, 0, Width, Height, NULL, NULL, HInstance, NULL);
}

void FWindow::ShowWindow()
{
}

class FWindowApplication
{
public:
	static void					CreateWindowsApplication(HINSTANCE hInstance);
	static FWindowApplication&	Get();

	shared_ptr<FWindow>			AddWindow(int width, int height);

private:
	static LRESULT CALLBACK	WindowProc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam);

private:
	static HINSTANCE						HInstance;
	std::vector<std::shared_ptr< FWindow>>	Windows;
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

	return WindowPtr;
}

int WINAPI WinMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd)
{
	// Preinit.

	FWindowApplication::CreateWindowsApplication(hInInstance);
	FWindowApplication::Get().AddWindow(900, 600);

	GNativeWindow = CreateMyWindow(900, 600, L"Hello world");
	InitRenderDevice();
	CreateBackBuffer(GNativeWindow, GNativeSwapChain, GRTV1);

	GWindow2 = CreateMyWindow(400, 300, L"Window2");
	CreateBackBuffer(GWindow2, GSwapChain2, GRTV2);

	GWindows.push_back(GNativeWindow);
	GWindows.push_back(GWindow2);

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
			float color[4] = { 0.3f, 0.3f, 1.0f, 1.0f };
			gDeviceContext->ClearRenderTargetView(GRTV1, color);
			GNativeSwapChain->Present(0, 0);

			color[0] = 1.0f;
			color[2] = 0.3f;
			gDeviceContext->ClearRenderTargetView(GRTV2, color);
			GSwapChain2->Present(0, 0);

			Sleep(30);
		}
	}

	// TODO. ReleaseContext.
	system("pause");
}