#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "dxgi.lib")
HWND	GNativeWindow;

IDXGISwapChain*			GSwapChain;
ID3D11Device*			GRenderDevice = nullptr;
ID3D11DeviceContext*	gDeviceContext;
ID3D11RenderTargetView*	GRTV = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CLOSE)
		PostQuitMessage(0);

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
	winclass.lpszClassName = name;

	if (!RegisterClass(&winclass))
		return NULL;

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exstyle = 0;
	HWND hwnd = CreateWindowEx(exstyle, name, name, style, 0, 0, width, height, NULL, NULL, winclass.hInstance, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	return hwnd;
}

bool InitRenderDevice()
{
	if (GRenderDevice)
		return true;

	int flags = D3D11_CREATE_DEVICE_DEBUG;

	//IDXGIFactory1* DXGIFactory1 = nullptr;
	//CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)DXGIFactory1);

	//HRESULT HResult = S_OK;
	//IDXGIAdapter* Adapter;
	//HResult = DXGIFactory1->EnumAdapters(0, &Adapter);

	//DXGI_ADAPTER_DESC AdapterDesc;
	//if (FAILED(Adapter->GetDesc(&AdapterDesc)))
	//	return false;

	D3D_FEATURE_LEVEL FeatureLevel;
	D3D_FEATURE_LEVEL ActualFeatureLevel = (D3D_FEATURE_LEVEL)0;
	D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
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

	RECT rect;
	::GetClientRect(GNativeWindow, &rect);
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rect.right - rect.left;
	sd.BufferDesc.Height = rect.bottom - rect.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = (HWND)window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	//if (factory->CreateSwapChain((ID3D11Device*)mD3DDevice, &sd, (IDXGISwapChain**)&mDefaultSwapChain) != 0)
	//	return _false;

	ID3D11Texture2D* buffer = nullptr;
	if (g->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer) != 0)
		return false;

	ID3D11RenderTargetView* rtv = nullptr;
	if (GRenderDevice->CreateRenderTargetView(buffer, nullptr, &rtv) != 0)
		return false;

	GRTV = rtv;

	return true;
}

int main()
{
	GNativeWindow = CreateMyWindow(900, 600, L"Hello world");
	InitRenderDevice();
	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float color[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
			gDeviceContext->ClearRenderTargetView(GRTV, color);

			GSwapChain->Present(0, 0);

			Sleep(30);
		}
	}
	system("pause");
}