#pragma once
#pragma warning(push)
#pragma warning(disable : 26812) // prefer enum class over enum

#ifndef RESONANT_FILTER_H
#define RESONANT_FILTER_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <json.hpp>

using nlohmann::json;

#ifndef sample
#define sample double
#endif

namespace dsp
{

	class resonant_filter
	{
	public:
		static const int MIN_ORDER = 2;
		static const int MAX_ORDER = 8;

		enum filter_type
		{
			HighPass = 0,
			LowPass,
			BandPass,
			BandStop,
			Count_filter_types
		};

		resonant_filter(double sampleRate = 44100.0);
		bool enabled = false;

		sample Process(const sample&);
		inline double normaliseFrequency(const double& frequency) { return std::min(0.99, std::max(0.001, 2.0 * frequency / sampleRate)); }
		inline double denormaliseFrequency(const double& norm) { return std::min(20000.0, std::max(20.0, (norm * sampleRate) / 2.0)); }

		inline void SetOrder(const int& order) { this->order = std::max(MIN_ORDER, std::min(MAX_ORDER, order)); }
		inline void SetCutoffHz(const double& hz) { SetCutoff(normaliseFrequency(hz)); }
		inline void SetCutoff(const double& cutoff) { this->cutoff = std::max(0.001, std::min(0.99, cutoff)); calculateFeedback(); }
		inline void SetResonance(const double& resonance) { this->resonance = std::max(0.0, std::min(0.99, resonance)); calculateFeedback(); }
		inline void SetType(filter_type type) { this->type = type; }

		inline int GetOrder() const { return order; }
		inline double GetCutoff() const { return cutoff; }
		inline double GetResonance() const { return resonance; }
		inline filter_type GetType() const { return type; }

		virtual json serializeParams() const;
		virtual void deserializeParams(const json& j);

	private:
		filter_type type = LowPass;
		int order = 3;
		double cutoff = 0.99;
		double resonance = 0.0;
		double feedback = 0.0;
		double buffer[resonant_filter::MAX_ORDER]{ 0.0 };
		double sampleRate = 44100.0;

		inline void calculateFeedback()
		{
			feedback = resonance + resonance / (1.0 - cutoff + std::numeric_limits<double>::epsilon());
			if (std::isnan(feedback)) feedback = 0.97;
		}
	};

}

#endif /* ifndef RESONANT_FILTER_H */

#pragma warning(pop)