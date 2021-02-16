#pragma once
#pragma warning(disable: 26812)

#ifndef WAVEGEN_H
#define WAVEGEN_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace dsp
{

	namespace wavegen
	{

#ifdef M_PI
		constexpr double PI = M_PI;
#else
		constexpr double PI = 3.14159265358979323846;
#endif

		enum wave_func
		{
			SINE = 0,
			SAWTOOTH,
			SQUARE,
			TRIANGLE,
			K_NUM_FUNCS
		};

		inline double Sine(const double& freq, const double& x, const double& amplitude)
		{
			return sin(2.0 * PI * freq * x) * amplitude;
		}

		inline double Sawtooth(const double& freq, const double& x, const double& amplitude, const int& harmonics)
		{
			double y = 0.0;
			for (int k = 1; k <= harmonics; k++)
				y += sin(2.0 * PI * freq * k * x) / (double)k;
			return (2.0 * amplitude) / PI * y;
		}

		inline double Square(const double& freq, const double& x, const double& amplitude, const int& harmonics)
		{
			double y = 0.0;
			for (int k = 1; k <= harmonics; k++)
				y += sin(2.0 * PI * (2.0 * k - 1.0) * freq * x) / (2.0 * k - 1.0);
			return (4.0 / PI) * y * amplitude;
		}

		inline double Triangle(const double& freq, const double& x, const double& amplitude, const int& harmonics)
		{
			double y = 0.0;
			double n;
			for (int k = 0; k < harmonics; k++)
			{
				n = 2.0 * k + 1.0;
				y += pow(-1.0, k) * pow(n, -2.0) * sin(2.0 * PI * freq * n * x);
			}
			return (8.0 / pow(PI, 2.0)) * y * amplitude;
		}

		inline double Generate(wave_func f, const double& freq, const double& x, const double& amplitude, const int& harmonics = 1)
		{
			switch (f)
			{
			case SINE: return Sine(freq, x, amplitude); break;
			case SAWTOOTH: return Sawtooth(freq, x, amplitude, harmonics); break;
			case SQUARE: return Square(freq, x, amplitude, harmonics); break;
			case TRIANGLE: return Triangle(freq, x, amplitude, harmonics); break;
			default: return 0.0;
			}
		}
	}

	//TODO - efficient approximations

}

#endif /* ifndef WAVEGEN_H */