#pragma once

namespace dsp
{

	template<class T>
	class onepole_lpf
	{
	public:
		onepole_lpf(double ms = 5.0, double sampleRate = 44100.0)
		{
			a = T(exp(-6.283185307179586476925286766559 / (ms * 0.001 * sampleRate)));
			b = T(1.0 - a);
			z = T(0.0);
		}

		~onepole_lpf() = default;

		T process(T in)
		{
			z = (in * b) + (z * a);
			return z;
		}

	private:
		T a;
		T b;
		T z;
	};

	template class onepole_lpf<float>;
	template class onepole_lpf<double>;


	template<class T>
	class ramp
	{
	public:
		ramp(unsigned window = 100U, T initial_value = T(0)) : 
			m_window(window), m_value(initial_value), m_target(initial_value) 
		{}

		T value() const { return m_value; }
		T target() const { return m_target; }

		void setValue(const T& value)
		{
			m_value = value;
			m_target = value;
		}

		void setTarget(const T& target)
		{
			m_target = target;
		}

		T run()
		{
			if (m_target != m_value)
			{
				T diff = m_target - m_value;
				m_value += diff * T(1.0) / (T)m_window;
			}
			return m_value;
		}

	private:
		unsigned m_window;
		T m_value;
		T m_target;
	};

	template class ramp<float>;
	template class ramp<double>;
}

