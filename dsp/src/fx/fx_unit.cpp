#include "fx/fx_unit.h"
#include "util.h"
#include <algorithm>

namespace dsp
{

	fx_unit::fx_unit(const int& id, const std::string& title, double sampleRate) :
		id(id), title(title), mix(0.0), sampleRate(sampleRate) {}

	void fx_unit::setSampleRate(const double& rate)
	{
		sampleRate = clamp(rate, 2.0, 192000.0);
	}

	void fx_unit::setMix(const double& m)
	{
		mix = clamp(m, 0.0, 1.0);
	}

}


