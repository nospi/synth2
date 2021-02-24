#include "ramp.h"

template<class T>
dsp::ramp<T>::ramp(unsigned window, T initial_value) : m_window(window), m_value(initial_value), m_target(initial_value) {}

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
	{
		T diff = m_target - m_value;
		m_value += diff * 1.0 / (T)m_window;
	}
	return m_value;
}
