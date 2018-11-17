#pragma once

#include "EngineBase.h"
#include "RHIResource.h"

class IDynamicRHI
{
public:
	virtual std::shared_ptr<FRHIViewport> RHICreateViewport(void* WindowHandle, bool FullScreen) = 0;
};

extern IDynamicRHI* GDynamicRHI;

__forceinline std::shared_ptr<FRHIViewport> RHICreateViewport(void* WindowHandle, bool FullScreen)
{
	return GDynamicRHI->RHICreateViewport(WindowHandle, FullScreen);
}