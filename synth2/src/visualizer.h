#pragma once
#include "ringbuf.h"

typedef ringbuffer<double> visualizer;

class visualizer_fft : public ringbuffer<double>
{
public:
	visualizer_fft(size_t size);
	void write(const double& val) override;
	double* dump(size_t& size) const override;

private:

	void processFFT();
	ringbuffer<double> postMem;
	ringbuffer<double> output;
};

