#pragma once

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "wavegen.h"
#include "note.h"
#include <json.hpp>
#include "ramp.h"

using nlohmann::json;

namespace dsp
{

	class oscillator
	{
	public:
		bool enabled = false;

	protected:
		ramp<double> noteOffset;
		ramp<double> centOffset;
		ramp<double> amplitude;
		ramp<double> phase;
		wavegen::wave_func func = wavegen::SINE;
		int harmonics = 4;

	public:
		virtual void setAmplitude(double amp);
		virtual void setPhase(double theta);
		virtual void setWaveFunc(wavegen::wave_func func);
		virtual void setHarmonics(int harms);
		virtual void setNoteOffset(int offset);
		virtual void setCentOffset(int offset);

		virtual double getAmplitude() const { return amplitude.target(); }
		virtual double getPhase() const { return phase.target(); }
		virtual wavegen::wave_func getWaveFunc() const { return func; }
		virtual int getHarmonics() const { return harmonics; }
		virtual int getNoteOffset() const { return (int)noteOffset.target(); }
		virtual int getCentOffset() const { return (int)centOffset.target(); }
		virtual int getTotalCentOffset() const { return (int)noteOffset.target() * 100 + (int)centOffset.target(); }

		virtual json serializeParams() const;
		virtual void deserializeParams(const json& j);

	public:
		oscillator(double amplitude = 1.0, double phase = 0.0, wavegen::wave_func func = wavegen::SINE, int harmonics = 4);
		virtual ~oscillator() = default;

		/*
		* For keyboard/note based synth - frequency will be calculated
		* based on note id and semitone/cent offsets.
		*/
		virtual double run(const note& n, const double& time);

	};

}

#endif /* ifndef OSCILLATOR_H */