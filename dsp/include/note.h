#pragma once

#ifndef NOTE_H
#define NOTE_H

#include "envelope.h"

namespace dsp
{

	class instrument_base;

	struct note
	{
		int id = 0;
		int offset = 0;
		double on = 0.0;
		double off = 0.0;
		double velocity = 1.0;
		bool active = false;
		envelope_adsr env;
		instrument_base* channel = nullptr;

		note();

		note(int id, int offset, double timeOn, double velocity = 1.0, instrument_base* channel = nullptr);

		bool operator==(const note& other) { return id == other.id; }
	};

}

#endif /* ifndef NOTE_H */