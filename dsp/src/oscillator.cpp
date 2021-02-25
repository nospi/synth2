#include "oscillator.h"
#include "scale.h"
#include "util.h"

namespace dsp
{

	oscillator::oscillator(double amplitude, double phase, wavegen::wave_func func, int harmonics) :
		enabled(false), harmonics(harmonics), func(func), 
		freq_ramp(10U), phase(441U, 0.0), amplitude(441U, 1.0),
		noteOffset(1000U, 0), centOffset(1000U, 0)
	{}

	void oscillator::setAmplitude(double amp)
	{
		amplitude.setTarget(clamp(amp, 0.0, 2.0));
	}

	void oscillator::setPhase(double theta)
	{
		phase.setTarget(clamp(theta, -1.0, 1.0));
	}

	void oscillator::setHarmonics(int harms)
	{
		harmonics = clamp(harms, 0, 24);
	}

	void oscillator::setNoteOffset(int offset)
	{
		noteOffset.setTarget(clamp(offset, -24, 24));
	}

	void oscillator::setCentOffset(int offset)
	{
		centOffset.setTarget(clamp(offset, -100, 100));
	}

	void oscillator::setWaveFunc(wavegen::wave_func waveform)
	{
		if ((int)waveform < 0 || (int)waveform >= (int)wavegen::wave_func::K_NUM_FUNCS)
			return;
		func = waveform;
	}

	double oscillator::run(const note& n, const double& time)
	{
		noteOffset.run();
		centOffset.run();
		amplitude.run();
		phase.run();
		double frequency = scale::noteToHz(n.id, noteOffset.value(), centOffset.value());
		return wavegen::Generate(func, frequency, time + phase.value() * 1.0 / frequency, std::min(1.0, amplitude.value()), harmonics);
	}

	json oscillator::serializeParams() const
	{
		return {
			{ "enabled", enabled },
			{ "amplitude", amplitude.target() },
			{ "noteOffset", noteOffset.target() },
			{ "centOffset", centOffset.target() },
			{ "phase", phase.target() },
			{ "func", func },
			{ "harmonics", harmonics }
		};
	}

	void oscillator::deserializeParams(const json& j)
	{
		enabled = j["enabled"];
		amplitude.setValue(j["amplitude"]);
		noteOffset.setValue(j["noteOffset"]);
		centOffset.setValue(j["centOffset"]);
		phase.setValue(j["phase"]);
		func = j["func"];
		harmonics = j["harmonics"];
	}
}