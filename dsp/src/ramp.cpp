#include "ramp.h"

template<class T>
dsp::ramp<T>::ramp(unsigned window) : m_window(window), m_value(T(0)), m_target(T(0)) {}

template<class T>
void dsp::ramp<T>::setValue(const T& value)
{
	m_value = value; 
	m_target = value;
}

template<class T>
void dsp::ramp<T>::setTarget(const T& target)
{
	m_target = target;
}

template<class T>
T dsp::ramp<T>::run()
{
	if (m_target != m_value)
		m_value += (m_target - m_value) / m_window;
	return m_value;
}
