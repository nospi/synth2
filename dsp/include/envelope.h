#pragma once

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <json.hpp>
#include "ramp.h"

using nlohmann::json;

namespace dsp
{

	class envelope_detector
	{
	public:
		envelope_detector(double window = 1.0, double sampleRate = 44100.0);
		virtual ~envelope_detector() = default;

		virtual double getWindow() const { return window; }
		virtual double getSampleRate() const { return sampleRate; }

		virtual void setWindow(double ms);
		virtual void setSampleRate(double sampleRate);

		inline void process(const double& in, double& state)
		{
			state = in + coeff * (state - in);
		}

	protected:
		double sampleRate;
		double window;
		double coeff;
		virtual void setCoeff();
	};


	class envelope_ar
	{
	public:
		envelope_ar(double att_ms, double rel_ms, double sampleRate = 44100.0);
		virtual ~envelope_ar() = default;

		virtual double getAttack() const { return att.getWindow(); }
		virtual double getRelease() const { return rel.getWindow(); }
		virtual double getSampleRate() const { return att.getSampleRate(); }

		virtual void setAttack(double ms);
		virtual void setRelease(double ms);
		virtual void setSampleRate(double sampleRate);

		inline void process(const double& in, double& state)
		{
			if (in > state)
				att.process(in, state);
			else
				rel.process(in, state);
		}

	protected:
		double DC_OFFSET;
		envelope_detector att;
		envelope_detector rel;
	};


	// TODO - switch from secs to ms
	class envelope_adsr
	{
		double DC_OFFSET;
		double attack;
		double decay;
		double sustain;
		double release;
		ramp<double> ramp;

	public:
		envelope_adsr(double attack = 0.03, double decay = 0.03, double sustain = 1.0, double release = 0.1);
		virtual ~envelope_adsr() = default;

		virtual void setAttack(double att);
		virtual void setDecay(double dec);
		virtual void setSustain(double sus);
		virtual void setRelease(double rel);

		virtual double getAttack() const { return attack; }
		virtual double getDecay() const { return decay; }
		virtual double getSustain() const { return sustain; }
		virtual double getRelease() const { return release; }

		virtual double process(const double& globalTime, const double& timeOn, const double& timeOff, const double& velocity);

		virtual json serializeParams() const;
		virtual void deserializeParams(const json& j);
	};

}


#endif /* ifndef ENVELOPE_H */