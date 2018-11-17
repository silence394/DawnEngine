#pragma once

#include "EngineBase.h"
#include "d3d11.h"
#include "DynamicRHI.h"

class FD3D11DynamicRHI : public IDynamicRHI
{
public:
	FD3D11DynamicRHI();

	ID3D11Device* GetDevice()
	{
		return D3DDevice;
	}

	ID3D11DeviceContext* GetDeviceContext()
	{
		return D3DContext;
	}

	virtual std::shared_ptr<class FRHIViewport> RHICreateViewport(void* WindowHandle, bool FullScreen);

private:
	IDXGIFactory1*			DXGIFactory = nullptr;
	ID3D11Device*			D3DDevice = nullptr;
	ID3D11DeviceContext*	D3DContext = nullptr;
};