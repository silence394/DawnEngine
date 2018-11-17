#include "D3D11RHI.h"
#include "D3D11Viewport.h"

FD3D11DynamicRHI::FD3D11DynamicRHI()
{
	int flags = 0;

	#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);

	IDXGIAdapter* Adapter;
	DXGIFactory->EnumAdapters(0, &Adapter);

	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED(Adapter->GetDesc(&AdapterDesc)))
		return;

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
		&D3DDevice,
		&ActualFeatureLevel,
		&D3DContext
	);
}

std::shared_ptr<FRHIViewport> FD3D11DynamicRHI::RHICreateViewport(void* WindowHandle, bool FullScreen)
{
	return std::make_shared<FRHIViewport>(FD3D11Viewport(this, WindowHandle, FullScreen));
}