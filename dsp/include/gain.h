#pragma once

#ifndef GAIN_H
#define GAIN_H

#include <cmath>
#include <limits>

namespace dsp
{

	/*
	* Desmos conversions
	* https://www.desmos.com/calculator/wcyh1zhpvy
	*/

	using lim = std::numeric_limits<double>;

	class gain
	{
	public:
		static inline double lin2dB(const double& linear)
		{
			static const double LOG_2_DB = 8.6858896380650365530225783783321;		// 20 / ln( 10 )
			return log(linear + lim::epsilon()) * LOG_2_DB;							// ln(linear) * 20 / ln(10)
		}

		static inline double linsq2dB(const double& linearSquared)
		{
			static const double LOGSQ_2_DB = 4.34294481903251750054550939239562;	// 10 / ln(10);
			return log(linearSquared + lim::epsilon()) * LOGSQ_2_DB;				// ln(linSq) * 10 / ln(10)
		}

		static inline double dB2lin(const double& dB)
		{
			static const double DB_2_LOG = 0.11512925464970228420089957273422;		// ln( 10 ) / 20
			return exp(dB * DB_2_LOG);												// e^(dB * ln(10) / 20)
		}
	};

}

#endif /* ifndef GAIN_H */