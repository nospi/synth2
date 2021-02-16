#pragma once

#include <string>

#include <imgui.h>
#include <imgui_internal.h>
#include <imstb_rectpack.h>
#include <imstb_textedit.h>
#include <imstb_truetype.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_pge.h>

#include <dsp.h>
#include "ringbuf.h"
#include "visualizer.h"
#include "instrument.h"

class synth2;

namespace gui
{
	void initialise();

	bool process(synth2*);

	namespace panel
	{
		bool visualizer(const char* label, const ringbuffer<double>& vis_waveform, const visualizer_fft& vis_fft);
		bool compressor(const char* label, dsp::compressor* comp = nullptr);
		bool resonant_filter(const char* label, dsp::resonant_filter* filt);
		bool oscillator(const char* label, dsp::oscillator* osc = nullptr);
		bool envelope_adsr(const char* label, dsp::envelope_adsr* env = nullptr);
		bool master(const char* label, synth2*);
	}

	namespace fx
	{
		bool fx_unit(instrument* inst, int slot);
		bool unit_selection(instrument* inst, int slot);
		bool mono_delay(dsp::mono_delay* unit);
	}

};

