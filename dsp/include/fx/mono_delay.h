#pragma once
#include "fx_unit.h"
#include "../resonant_filter.h"

namespace dsp
{

	class mono_delay : public fx_unit
	{
	public:
		const double MAX_DELAY_TIME = 4.0;

	public:
		mono_delay(const double& time = 0.5, const double& feedback = 0.3, const double& sampleRate = 44100.0);
		~mono_delay() override;

		virtual void setDelayTime(const double& time);
		virtual void setFeedback(const double& feedback);
		virtual void setLoCut(const double& hz);
		virtual void setHiCut(const double& hz);

		virtual double getDelayTime() const { return time; }
		virtual double getFeedback() const { return feedback; }
		virtual double getLoCut() const { return hpf.GetCutoff() * (sampleRate / 2); }
		virtual double getHiCut() const { return lpf.GetCutoff() * (sampleRate / 2); }

		double process(const double& in) override;
		bool deserializeParams(const json& j) override;
		json serializeParams() override;

	protected:
		double time = 0.5;
		double feedback = 0.0;
		int phase = 0;
		resonant_filter hpf;
		resonant_filter lpf;

	private:
		int bufferSize = 0;
		double* buffer = nullptr;
	};

}

