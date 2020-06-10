#pragma once
#include <vector>

template<class T>
bool contains(T data, std::vector<T>& list)
{
	for (T t : list)
		if (t == data)
			return true;

	return false;
}
