#include "fx/mono_delay.h"
#include "util.h"
#include <iostream>

namespace dsp
{

	mono_delay::mono_delay(const double& time, const double& feedback, const double& sampleRate) :
		fx_unit(1, "Mono Delay", sampleRate), time(time), feedback(feedback),
		hpf(sampleRate), lpf(sampleRate)
	{
		buffer = new double[(size_t)(MAX_DELAY_TIME * sampleRate)]{ 0.0 };
		bufferSize = (int)(time * sampleRate);
		mix = 0.3;
		enabled = true;

		hpf.SetType(resonant_filter::HighPass);
		hpf.SetOrder(8);
		hpf.SetCutoffHz(300.0);
		hpf.SetResonance(0.2);
		hpf.enabled = true;

		lpf.SetType(resonant_filter::LowPass);
		lpf.SetOrder(4);
		lpf.SetCutoffHz(8000.0);
		lpf.SetResonance(0.1);
		lpf.enabled = true;
	}

	mono_delay::~mono_delay()
	{
		if (buffer != nullptr)
			delete[] buffer;
	}

	void mono_delay::setDelayTime(const double& delayTime)
	{
		time = clamp(delayTime, 0.05, MAX_DELAY_TIME);
		bufferSize = (int)(time * sampleRate);
	}

	void mono_delay::setFeedback(const double& fb)
	{
		feedback = clamp(fb, 0.0, 0.97);
	}

	void mono_delay::setLoCut(const double& hz)
	{
		hpf.SetCutoffHz(hz);
	}

	void mono_delay::setHiCut(const double& hz)
	{
		lpf.SetCutoffHz(hz);
	}

	double mono_delay::process(const double& in)
	{
		phase %= bufferSize;
		double out = buffer[phase];
		buffer[phase++] = in + out * feedback;
		out = hpf.Process(lpf.Process(out));
		return out * mix + in * (1.0 - mix);
	}

	bool mono_delay::deserializeParams(const json& j)
	{
		try {
			if (j.contains("time")) setDelayTime(j["time"]);
			if (j.contains("feedback")) setFeedback(j["feedback"]);
			if (j.contains("mix")) setMix(j["mix"]);
			if (j.contains("hpf")) hpf.deserializeParams(j["hpf"]);
			if (j.contains("lpf")) lpf.deserializeParams(j["lpf"]);
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}

	json mono_delay::serializeParams()
	{
		return { 
			{ "time", time },
			{ "feedback", feedback },
			{ "mix", mix },
			{ "hpf", hpf.serializeParams() },
			{ "lpf", lpf.serializeParams() }
		};
	}
}