#pragma once

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "wavegen.h"
#include "note.h"
#include <json.hpp>

using nlohmann::json;

namespace dsp
{

	class oscillator
	{
	public:
		bool enabled = false;

	protected:
		int noteOffset = 0;
		int centOffset = 0;
		double amplitude = 0.5;
		double phase = 0.0;
		wavegen::wave_func func = wavegen::SINE;
		int harmonics = 4;

	public:
		virtual void setAmplitude(double amp);
		virtual void setPhase(double theta);
		virtual void setWaveFunc(wavegen::wave_func func);
		virtual void setHarmonics(int harms);
		virtual void setNoteOffset(int offset);
		virtual void setCentOffset(int offset);

		virtual double getAmplitude() const { return amplitude; }
		virtual double getPhase() const { return phase; }
		virtual wavegen::wave_func getWaveFunc() const { return func; }
		virtual int getHarmonics() const { return harmonics; }
		virtual int getNoteOffset() const { return noteOffset; }
		virtual int getCentOffset() const { return centOffset; }
		virtual int getTotalCentOffset() const { return noteOffset * 100 + centOffset; }

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