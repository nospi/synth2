#include "fft.h"
#include "gain.h"
#include "util.h"

namespace dsp
{

    void fft::fft(double* x_in, std::complex<double>* x_out, int nBufSize)
    {
        for (int i = 0; i < nBufSize; i++)
        {
            x_out[i] = std::complex<double>(x_in[i], 0);
            x_out[i] *= 1;  // window
        }
        fft::fft_rec(x_out, nBufSize);
    }

    void fft::fft_rec(std::complex<double>* x, int nBufSize)
    {
        if (nBufSize <= 1) return;

        // fft
        std::complex<double>* odd = new std::complex<double>[nBufSize / 2];
        std::complex<double>* even = new std::complex<double>[nBufSize / 2];
        for (int i = 0; i < nBufSize / 2; i++)
        {
            even[i] = x[i * 2];
            odd[i] = x[i * 2 + 1];
        }
        fft::fft_rec(even, nBufSize / 2);
        fft::fft_rec(odd, nBufSize / 2);

        // dft
        for (int k = 0; k < nBufSize / 2; k++)
        {
            std::complex<double> t = odd[k] * exp(std::complex<double>(0, -2 * PI * k / nBufSize));
            x[k] = even[k] + t;
            x[nBufSize / 2 + k] = even[k] - t;
        }

        // cleanup
        delete[] odd;
        delete[] even;
    }

    void fft::fft_magnitude(double* in, double* out, const int nBufSize)
    {
        std::complex<double>* c = new std::complex<double>[nBufSize];
        if (c == nullptr) return;
        fft::fft(in, c, nBufSize);
        for (int i = 0; i < nBufSize / 2; i++)
            out[i] = sqrt(c[i].real() * c[i].real() + c[i].imag() * c[i].imag());
        delete[] c;
    }

    void fft::fft_magnitude_db(double* in, double* out, int nBufSize)
    {
        std::complex<double>* c = new std::complex<double>[nBufSize];
        if (c == nullptr) return;
        fft::fft(in, c, nBufSize);
        for (int i = 0; i < nBufSize / 2; i++)
            out[i] = gain::linsq2dB(c[i].real() * c[i].real() + c[i].imag() * c[i].imag());
        delete[] c;
    }
}