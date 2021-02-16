#pragma once

#ifndef FFT_H
#define FFT_H

#define _USE_MATH_DEFINES
#include <complex>

namespace dsp 
{

	namespace fft
	{

#ifdef M_PI
		const double PI = M_PI;
#else
		const double PI = std::atan(1.0) * 4;
#endif

		void fft(double* x_in, std::complex<double>* x_out, int nBufSize);
		void fft_rec(std::complex<double>* x, int nBufSize);
		void fft_magnitude(double* in, double* out, const int nBufSize);
		void fft_magnitude_db(double* in, double* out, int nBufSize);
	}

}

#endif /* ifndef FFT_H */