#pragma once

#ifndef LIMITER_H
#define LIMITER_H

#include "envelope.h"
#include "util.h"

namespace dsp
{

	class limiter : public envelope_ar
	{
	public:
		limiter(double sampleRate = 44100.0);
		virtual ~limiter() = default;
		double process(const double& in);

		bool enabled = true;

		virtual void setInputDrive(const double& dB);
		virtual void setOutputDrive(const double& dB);
		virtual double getInputDrive() const { return driveIn_dB; }
		virtual double getOutputDrive() const { return driveOut_dB; }
		virtual double getGainReduction_dB() const { return gainReduction_dB; }

	protected:
		const double threshold_dB = -0.3;
		double driveIn_dB = 0.0;
		double driveOut_dB = 0.0;
		double gainReduction_dB = 0.0;

	private:
		double env_dB;
	};

}

#endif /* ifndef LIMITER_H */

