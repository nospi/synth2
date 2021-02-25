#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <memory>

namespace dsp
{

	template<class T>
	T clamp(T v, T lo, T hi)
	{
		if (lo > hi) std::swap(lo, hi);
		if (v != v)	v = T(0);	// v is NaN
		return (v < lo) ? lo : ((v > hi) ? hi : v);
	}

	template int clamp<int>(int, int, int);
	template float clamp<float>(float, float, float);
	template double clamp<double>(double, double, double);


	template<class T>
	T lerp(const T& from, const T& to, const T& t)
	{
		if (t != t) return from; // t is NaN
		return from + t * (to - from);
	}

	template int lerp<int>(const int&, const int&, const int&);
	template float lerp<float>(const float&, const float&, const float&);
	template double lerp<double>(const double&, const double&, const double&);

}

#endif /* ifndef UTIL_H */