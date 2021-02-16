#pragma once

#ifndef INSTRUMENT_BASE_H
#define INSTRUMENT_BASE_H

#include <json.hpp>
#include "note.h"

using nlohmann::json;

namespace dsp
{

	class instrument_base
	{
	public:
		instrument_base(const int& channels = 1, const double& sampleRate = 44100.0);
		virtual ~instrument_base() = default;
		virtual int getChannels() const { return channels; }
		virtual double getSampleRate() const { return sampleRate; }
		virtual double sound(const double& time, note& n, const int& channel, bool &noteFinished) = 0;
		virtual void sound(const double& time, note& n, double* out, bool &noteFinished);
		virtual bool deserializeParams(const json& j) = 0;
		virtual json serializeParams() const = 0;

	private:
		int channels = 1;
		double sampleRate = 44100.0;
	};

}

#endif /* ifndef INSTRUMENT_BASE_H */