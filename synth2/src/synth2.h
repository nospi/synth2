#pragma once

#include <string>
#include <vector>
#include <mutex>

#include <olcPixelGameEngine.h>
#include <olcPGEX_Sound.h>
#include <dsp.h>
#include <RtMidi.h>

#include "ringbuf.h"
#include "gui.h"
#include "instrument.h"

class synth2 : public olc::PixelGameEngine
{

public:
	struct parameters
	{
		unsigned channels = 1U;
		unsigned sampleRate = 44100U;
		unsigned blocks = 8U;
		unsigned bufferSize = 512U;
		unsigned deviceId = 0U;
		bool midiEnabled = true;
		unsigned midiPort = 0U;
	};

	struct noteData
	{
		std::mutex* mutex = nullptr;
		std::vector<dsp::note>* activeNotes = nullptr;
		instrument* voice = nullptr;
	};

public:
	parameters params;
	instrument inst;
	visualizer vis_waveform;
	visualizer_fft vis_fft;

private:
	olc::imgui::PGE_ImGUI pge_imgui;
	std::mutex muxNotes;
	std::vector<note> activeNotes;
	std::vector<olc::Key> vKeys = { olc::Z, olc::S, olc::X, olc::D, olc::C, olc::V, olc::G, olc::B, olc::H, olc::N, olc::J, olc::M, olc::COMMA, olc::L, olc::PERIOD };

public:
	synth2(parameters& config);
	~synth2() = default;
	
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float) override;

private:
	bool initialiseMidi();
	bool initialiseAudio(unsigned sampleRate, unsigned channels, unsigned blocks, unsigned bufferSize);
	
	// handle keyboard input if MIDI is not in use
	void processQwertyInput();
	
	// per channel audio process hook
	double makeNoise(int channel, double globalTime, double timeStep);
	
	// multi-channel audio process hook
	void makeMuchNoise(double* out, double globalTime, double timeStep);
	
};

