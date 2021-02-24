#pragma once

namespace dsp
{

	template<class T>
	class ramp
	{
	public:
		ramp(unsigned window = 100U, T initial_value = T(0));

		T value() const { return m_value; }
		T target() const { return m_target; }

		void setValue(const T& value);
		void setTarget(const T& target);

		T run();

	private:
		unsigned m_window;
		T m_value;
		T m_target;
	};

	template class ramp<float>;
	template class ramp<double>;
}

