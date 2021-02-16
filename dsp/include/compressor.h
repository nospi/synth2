#pragma once

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "gain.h"
#include "envelope.h"

namespace dsp
{

	class compressor : public envelope_ar
	{
	public:
		compressor(double sampleRate = 44100.0);
		virtual ~compressor() = default;

		bool enabled = false;
		bool autoMakeupGain = false;

		virtual double getGainReduction() const { return gainReduction_dB; }
		virtual double getInput() const { return input_dB; }
		virtual double getOutput() const { return output_dB; }
		virtual double getThreshold() const { return threshold_dB; }
		virtual double getRatio() const { return ratio; }
		virtual double getMakeupGain() const { return makeupGain_dB; }

		virtual void setThreshold(double dB);
		virtual void setRatio(double ratio);
		virtual void setMakeupGain(double dB);

		virtual void init();
		virtual double process(const double& in);
		virtual json serializeParams() const;
		virtual void deserializeParams(const json& j);

	private:
		double threshold_dB = 0.0;
		double makeupGain_dB = 0.0;
		double peakGain_dB = 0.0;
		double ratio = 1.0;
		double env_dB = 0.0;

		// metering varibles
		double gainReduction_dB = 0.0;
		double input_dB = 0.0;
		double output_dB = 0.0;

	};

}

#endif COMPRESSOR_H /* ifndef COMPRESSOR_H */