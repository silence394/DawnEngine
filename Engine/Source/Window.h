#pragma once

class IWindow
{
public:
	virtual void Tick() = 0;
	virtual void Show() = 0;
};