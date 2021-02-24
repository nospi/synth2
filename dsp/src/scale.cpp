#include "scale.h"

std::string dsp::scale::noteName(int noteId)
{
	const char* notes[] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	noteId -= 21; // A0 is MIDI note id 21 - as our array begins at A, we should ensure our noteIds do too
	if (noteId < 0) noteId = 0;
	int octave = noteId / 12;
	int idx = noteId % 12;
	return std::string(notes[idx]) + std::to_string(octave);
}
