#include "limiter.h"
#include "gain.h"

dsp::limiter::limiter(double sampleRate) :
	envelope_ar(10, 100, sampleRate),
	env_dB(DC_OFFSET)
{}

double dsp::limiter::process(const double& in)
{
	// apply input amplification
	double signal = in * gain::dB2lin(driveIn_dB);

	// rectify and determine dB over threshold
	double key_dB = gain::lin2dB(std::abs(signal));
	double over_dB = std::max(0.0, key_dB - threshold_dB);

	// process attack/release envelope
	envelope_ar::process(over_dB + DC_OFFSET, env_dB);
	over_dB = env_dB - DC_OFFSET;
	gainReduction_dB = -over_dB;

	// apply gain reduction and output drive
	signal *= gain::dB2lin(gainReduction_dB) * gain::dB2lin(driveOut_dB);
	
	return signal;
}

void dsp::limiter::setInputDrive(const double& dB)
{
	driveIn_dB = clamp(dB, -48.0, 12.0);
}

void dsp::limiter::setOutputDrive(const double& dB)
{
	driveOut_dB = clamp(dB, -48.0, 12.0);
}
