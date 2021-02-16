#include "resonant_filter.h"

namespace dsp
{

	resonant_filter::resonant_filter(double sampleRate) : 
		cutoff(0.99), resonance(0.0), type(LowPass), sampleRate(sampleRate)
	{
		for (int i = 0; i < MAX_ORDER; i++)
			buffer[i] = 0.0;
		calculateFeedback();
	}

	double resonant_filter::Process(const double& in)
	{
		buffer[0] += cutoff * (in - buffer[0] + feedback * (buffer[0] - buffer[1]));
		for (int i = 1; i < order; i++)
			buffer[i] += cutoff * (buffer[i - 1] - buffer[i]);

		switch (type)
		{
		case LowPass: return buffer[order - 1];
		case HighPass: return in - buffer[order - 1];
		case BandPass: return buffer[0] - buffer[order - 1];
		case BandStop: return in - (buffer[0] - buffer[order - 1]);
		default: return 0.0;
		}
	}

	json resonant_filter::serializeParams() const
	{
		return {
			{ "enabled", enabled },
			{ "type", type },
			{ "order", order },
			{ "cutoff", cutoff },
			{ "resonance", resonance }
		};
	}

	void resonant_filter::deserializeParams(const json& j)
	{
		enabled = j["enabled"];
		SetType(j["type"]);
		SetOrder(j["order"]);
		SetCutoff(j["cutoff"]);
		SetResonance(j["resonance"]);
	}

}