#include "note.h"

namespace dsp
{
	note::note() :
		id(0), on(0.0), off(0.0),
		velocity(1.0), channel(nullptr),
		active(false), offset(0) {}

	note::note(int id, int offset, double timeOn, double velocity, instrument_base* channel) :
		id(id), offset(offset), on(timeOn), off(0.0),
		velocity(velocity), channel(channel),
		active(true) {}
}
