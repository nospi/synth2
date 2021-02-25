#include "visualizer.h"
#include <fft.h>
#include <util.h>

visualizer_fft::visualizer_fft(size_t size) :
	ringbuffer(size),
	postMem(size),
	output(size / 2)
{}

void visualizer_fft::write(const double& val)
{
	ringbuffer::write(val);
	if (phase == size)
	{
		//after last index has been written, before phase %= size
		processFFT();
	}
}

double* visualizer_fft::dump(size_t& size) const
{
	return output.dump(size);
}

void visualizer_fft::processFFT()
{
	size_t sz = 0;
	size_t sz_p = 0;
	dsp::fft::fft_magnitude_db(ringbuffer::dump(sz), postMem.dump(sz_p), (int)sz);
	// populate output buffer with fft output bins; use logarithmic array indexing
	output.copy_from(postMem.dump(sz_p), 0, sz_p / 2, true);
}
