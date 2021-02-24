#include "scale.h"

std::string dsp::scale::noteName(int noteId)
{
	const char* notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
	noteId -= 12; // C0 is MIDI note id 12
	if (noteId < 0) noteId = 0;
	int octave = noteId / 12;
	int idx = noteId % 12;
	return std::string(notes[idx]) + std::to_string(octave);
}
