#pragma once
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <util.h>
#include <cmath>

template<class T>
class ringbuffer
{
protected:
	T* buf = nullptr;
	size_t size = 0;
	size_t phase = 0;

public:
	ringbuffer(size_t size) : size(size), phase(0)
	{
		buf = new T[size]{ T(0) };
	}

	virtual ~ringbuffer()
	{
		if (buf != nullptr)
			delete[] buf;
		buf = nullptr;
	}

	virtual void write(const T& val)
	{
		phase %= size;
		buf[phase++] = val;
	}

	virtual T read(size_t index)
	{
		if (buf != nullptr && 0 <= index && index < size)
			return buf[index];
		return T(0);
	}

	virtual T* dump(size_t& size) const
	{
		size = this->size;
		return buf;
	}

	virtual bool copy_from(T* other, size_t offset, size_t size, bool lin2log = false)
	{
		if (other == nullptr || size > this->size)
			return false;
		for (int i = 0; i < size; i++)
			if (lin2log)
			{
				/* https://www.desmos.com/calculator/4acmrlovkg */
				int j = dsp::clamp((int)exp(((i + 1) / (T)size) * log((T)size)) - 1, 0, (int)size - 1);
				buf[i] = other[j + offset];
			}
			else
				buf[i] = other[i + offset];
		return true;
	}

	virtual size_t get_phase() const { return phase; }
};

template class ringbuffer<int>;
template class ringbuffer<float>;
template class ringbuffer<double>;

#endif /* ifndef RINGBUFFER_H */