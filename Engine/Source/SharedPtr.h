#pragma once

#include <memory>

template<typename T>
class TSharedPtr : public std::shared_ptr<T>
{
public:
	TSharedPtr() {}

	operator T*() const
	{
		return std::shared_ptr<T>::get();
	}
};