#include "compressor.h"
#include "util.h"
#include <limits>

namespace dsp
{

	compressor::compressor(double sampleRate) :
		envelope_ar(10.0, 100.0, sampleRate),
		threshold_dB(0.0),
		ratio(1.0),
		env_dB(std::numeric_limits<double>::epsilon())
	{}

	void compressor::setThreshold(double dB)
	{
		threshold_dB = dB;
		peakGain_dB = 0.0;
	}

	void compressor::setRatio(double ratio)
	{
		this->ratio = std::max(DC_OFFSET, ratio);
	}

	void compressor::setMakeupGain(double dB)
	{
		makeupGain_dB = dB;
		autoMakeupGain = false;
	}

	void compressor::init()
	{
		env_dB = DC_OFFSET;
	}

	double compressor::process(const double& in)
	{
		// rectify and convert to dB
		input_dB = gain::lin2dB(std::abs(in));
		
		// calculate excess signal
		double over_dB = std::max(0.0, input_dB - threshold_dB);

		// run envelope
		envelope_ar::process(over_dB + DC_OFFSET, env_dB);
		over_dB = env_dB - DC_OFFSET;

		// perform transfer function
		gainReduction_dB = over_dB * (ratio - 1.0);
		
		// apply gain reduction
		double out = in * gain::dB2lin(gainReduction_dB);
	
		if (autoMakeupGain)
		{
			// if calc'd gain is above peak re-set the makeupGain
			if (-gainReduction_dB > peakGain_dB)
			{
				makeupGain_dB = -gainReduction_dB;
				peakGain_dB = makeupGain_dB;
			}
		}
		
		// apply makeup gain
		out *= gain::dB2lin(makeupGain_dB);
		output_dB = gain::lin2dB(out);

		return out;
	}

	json compressor::serializeParams() const
	{
		return {
			{ "enabled", enabled },
			{ "autoMakeupGain", autoMakeupGain },
			{ "makeupGain", makeupGain_dB },
			{ "attack", getAttack() },
			{ "release", getRelease() },
			{ "ratio", getRatio() },
			{ "threshold", getThreshold() }
		};
	}

	void compressor::deserializeParams(const json& j)
	{
		if (j.contains("enabled")) enabled = j["enabled"];
		if (j.contains("autoMakeupGain")) autoMakeupGain = j["autoMakeupGain"];
		if (j.contains("makeupGain")) makeupGain_dB = j["makeupGain"];
		if (j.contains("attack")) setAttack(j["attack"]);
		if (j.contains("release")) setRelease(j["release"]);
		if (j.contains("ratio")) setRatio(j["ratio"]);
		if (j.contains("threshold")) setThreshold(j["threshold"]);
	}

}
