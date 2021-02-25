#pragma once
#pragma warning(disable: 26812)

#ifndef SCALE_H
#define SCALE_H

#include <cmath>
#include <algorithm>
#include <string>

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

		inline double noteToHz(int noteId, double noteOffset = 0.0, double centOffset = 0.0, tuning_system system = equal_temperament, double concertPitch = 440.0)
		{
			switch (system)
			{
			case equal_temperament:
				return concertPitch * pow(1.0594630943592952645618252949463, noteId + noteOffset - 69) * pow(1.0005777895065548488418016859213, centOffset);
			case just_intonation:
			default:
				return 0.0;
			}
		}

		inline int hzToNoteId(double hz, tuning_system system = equal_temperament, double concertPitch = 440.0)
		{
			switch (system)
			{
			case equal_temperament:
			{
				hz = std::ceil(hz * 100.0) / 100.0; // round to 2 decimal places
				return (int)(12 * log2(hz / concertPitch) + 69);
			}
			case just_intonation:
			default:
				return 0;
			}
		}

		std::string noteName(int noteId);
	};
}

#endif /* ifndef SCALE_H */