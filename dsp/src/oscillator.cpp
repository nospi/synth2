#include "oscillator.h"
#include "scale.h"
#include "util.h"

namespace dsp
{

	oscillator::oscillator(double amplitude, double phase, wavegen::wave_func func, int harmonics) :
		amplitude(amplitude), phase(phase), enabled(false),
		harmonics(harmonics), func(func) {}

	void oscillator::setAmplitude(double amp)
	{
		amplitude = clamp(amp, 0.0, 2.0);
	}

	void oscillator::setPhase(double theta)
	{
		phase = clamp(theta, -1.0, 1.0);
	}

	void oscillator::setHarmonics(int harms)
	{
		harmonics = clamp(harms, 0, 24);
	}

	void oscillator::setNoteOffset(int offset)
	{
		noteOffset = clamp(offset, -24, 24);
	}

	void oscillator::setCentOffset(int offset)
	{
		centOffset = clamp(offset, -100, 100);
	}

	void oscillator::setWaveFunc(wavegen::wave_func waveform)
	{
		if ((int)waveform < 0 || (int)waveform >= (int)wavegen::wave_func::K_NUM_FUNCS)
			return;
		func = waveform;
	}

	double oscillator::run(const note& n, const double& time)
	{
		double frequency = scale::noteToHz(n.id, noteOffset, centOffset);
		return wavegen::Generate(func, frequency, time + phase * 1.0 / frequency, std::min(1.0, amplitude), harmonics);
	}

	json oscillator::serializeParams() const
	{
		return {
			{ "enabled", enabled },
			{ "amplitude", amplitude },
			{ "noteOffset", noteOffset },
			{ "centOffset", centOffset },
			{ "phase", phase },
			{ "func", func },
			{ "harmonics", harmonics }
		};
	}

	void oscillator::deserializeParams(const json& j)
	{
		enabled = j["enabled"];
		amplitude = j["amplitude"];
		noteOffset = j["noteOffset"];
		centOffset = j["centOffset"];
		phase = j["phase"];
		func = j["func"];
		harmonics = j["harmonics"];
	}
}