#pragma once
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

template<class T>
class ringbuffer
{
protected:
	T* buf = nullptr;
	size_t size = 0;
	size_t phase = 0;

public:
	ringbuffer(size_t size);
	virtual ~ringbuffer();

	virtual void write(const T& val);
	virtual T read(size_t index);
	virtual T* dump(size_t& size) const;
	virtual bool copy_from(T* other, size_t offset, size_t size, bool lin2log = false);
	virtual size_t get_phase() const { return phase; }
};

template class ringbuffer<int>;
template class ringbuffer<float>;
template class ringbuffer<double>;

#endif /* ifndef RINGBUFFER_H */