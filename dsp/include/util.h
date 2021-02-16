#pragma once

#ifndef UTIL_H
#define UTIL_H

namespace dsp
{

	template<class T>
	T clamp(T v, T min, T max);

	template<class T>
	T lerp(const T& from, const T& to, const T& t);

}

#endif /* ifndef UTIL_H */