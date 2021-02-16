#pragma once
#pragma warning(disable: 26812)

#ifndef SCALE_H
#define SCALE_H

#include <cmath>
#include <algorithm>

namespace dsp
{

	namespace scale
	{
		enum tuning_system
		{
			equal_temperament = 0,
			just_intonation,
			k_num_tuning_systems
		};

		inline double noteToHz(int noteId, int noteOffset = 0, int centOffset = 0, tuning_system system = equal_temperament)
		{
			switch (system)
			{
			case equal_temperament:
				return 8 * pow(1.0594630943592952645618252949463, noteId + noteOffset) * pow(1.0005777895065548488418016859213, centOffset);
			case just_intonation:
			default:
				return 0.0;
			}
		}

		//inline int hzToNote(double hz);
	};
}

#endif /* ifndef SCALE_H */