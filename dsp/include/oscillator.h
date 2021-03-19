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
		void setAmplitude(double amp);
		void setPhase(double theta);
		void setWaveFunc(wavegen::wave_func func);
		void setHarmonics(int harms);
		void setNoteOffset(int offset);
		void setCentOffset(int offset);

		double getAmplitude() const { return amplitude.target(); }
		double getPhase() const { return phase.target(); }
		wavegen::wave_func getWaveFunc() const { return func; }
		int getHarmonics() const { return harmonics; }
		int getNoteOffset() const { return (int)noteOffset.target(); }
		int getCentOffset() const { return (int)centOffset.target(); }
		int getTotalCentOffset() const { return (int)noteOffset.target() * 100 + (int)centOffset.target(); }

		json serializeParams() const;
		void deserializeParams(const json& j);

	public:
		oscillator(double amplitude = 1.0, double phase = 0.0, wavegen::wave_func func = wavegen::SINE, int harmonics = 4);
		virtual ~oscillator() = default;

		/*
		* For keyboard/note based synth - frequency will be calculated
		* based on note id and semitone/cent offsets.
		*/
		double run(const note& n, const double& time);

	};

}

#endif /* ifndef OSCILLATOR_H */