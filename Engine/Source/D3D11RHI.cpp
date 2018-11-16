#include "D3D11RHI.h"
#include "d3d11.h"

FD3D11DynamicRHI::FD3D11DynamicRHI()
{
	int flags = 0;

	#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	IDXGIFactory1* Factory = DXGIFactory.get();

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&Factory);

	IDXGIAdapter* Adapter;
	Factory->EnumAdapters(0, &Adapter);

	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED(Adapter->GetDesc(&AdapterDesc)))
		return;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL ActualFeatureLevel = (D3D_FEATURE_LEVEL)0;

	ID3D11Device* Device = D3DDevice.get();
	ID3D11DeviceContext* context = D3DContext.get();

	D3D11CreateDevice(
		Adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		flags,
		&FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&Device,
		&ActualFeatureLevel,
		&context
	);
}