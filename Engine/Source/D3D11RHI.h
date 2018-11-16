#pragma once

#include "EngineBase.h"
#include "RHIInterface.h"
#include "d3d11.h"

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

private:
	TSharedPtr<IDXGIFactory1>		DXGIFactory;
	TSharedPtr<ID3D11Device>		D3DDevice;
	TSharedPtr<ID3D11DeviceContext>	D3DContext;
};