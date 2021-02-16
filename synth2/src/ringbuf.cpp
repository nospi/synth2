#include "ringbuf.h"
#include <util.h>
#include <cmath>

template<class T>
ringbuffer<T>::ringbuffer(size_t size) : size(size), phase(0)
{
	buf = new T[size]{ T(0) };
}

template<class T>
ringbuffer<T>::~ringbuffer()
{
	if (buf != nullptr)
		delete[] buf;
	buf = nullptr;
}

template<class T>
void ringbuffer<T>::write(const T& val)
{
	phase %= size;
	buf[phase++] = val;
}

template<class T>
T ringbuffer<T>::read(size_t index)
{
	if (buf != nullptr && 0 <= index && index < size)
		return buf[index];
	return T(0);
}

template<class T>
T* ringbuffer<T>::dump(size_t& size) const
{
	size = this->size;
	return buf;
}

template<class T>
bool ringbuffer<T>::copy_from(T* other, size_t offset, size_t size, bool lin2log)
{
	if (other == nullptr || size > this->size)
		return false;
	for (int i = 0; i < size; i++)
		if (lin2log)
		{
			/* https://www.desmos.com/calculator/4acmrlovkg */
			int j = dsp::clamp((int)exp(((i + 1) / (double)size) * log((double)size)) - 1, 0, (int)size - 1);
			buf[i] = other[j + offset];
		}
		else
			buf[i] = other[i + offset];
	return true;
}
