#pragma once

#include "RHIResource.h"

class FD3D11Viewport : public FRHIViewport
{
public:
	FD3D11Viewport(class FD3D11DynamicRHI* D3DRHI, void* WindowHandle, bool FullScreen);

	virtual void	Present();
};