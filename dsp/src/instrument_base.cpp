#include "instrument_base.h"

namespace dsp
{

	instrument_base::instrument_base(const int& channels, const double& sampleRate) :
		channels(channels), sampleRate(sampleRate) 
	{}

	void instrument_base::sound(const double& time, note& n, double* out, bool& noteFinished)
	{
		if (out != nullptr)
			for (int c = 0; c < channels; c++)
				out[c] = sound(time, n, c, noteFinished);
	}
}
