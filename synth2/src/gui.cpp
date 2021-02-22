#include "gui.h"
#include "synth2.h"
#include "preset.h"

#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>

std::string menu_action = "";


void gui::initialise()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}


bool gui::process(synth2* app)
{
	ImGui::DockSpaceOverViewport();

	// draw main menu bar
	ImGui::BeginMainMenuBar();
	ImGui::Text("[synth2]");
	if (ImGui::MenuItem("Load")) menu_action = "load";
	if (ImGui::MenuItem("Save")) menu_action = "save";
	if (ImGui::MenuItem("Reset")) menu_action = "reset";
	ImGui::EndMainMenuBar();

	// process main menu bar input
	if (menu_action == "load")
	{
		ImGuiFileDialog::Instance()->OpenDialog("dlg_load", "Load Preset", ".pst", ".");
		if (ImGuiFileDialog::Instance()->Display("dlg_load", ImGuiWindowFlags_NoDocking))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				json j;
				if (ReadJSONFromFile(ImGuiFileDialog::Instance()->GetFilePathName(), j))
					app->inst.deserializeParams(j);
			}
			ImGuiFileDialog::Instance()->Close();
			menu_action = "";
		}
	}

	if (menu_action == "save")
	{
		ImGuiFileDialog::Instance()->OpenDialog("dlg_save", "Save Preset", ".pst", app->inst.title + ".", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
		if (ImGuiFileDialog::Instance()->Display("dlg_save", ImGuiWindowFlags_NoDocking))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
				WriteJSONToFile(ImGuiFileDialog::Instance()->GetFilePathName(), app->inst.serializeParams());
			ImGuiFileDialog::Instance()->Close();
			menu_action = "";
		}
	}

	if (menu_action == "reset")
	{
		if (ImGui::BeginPopupModal("Confirm Reset"))
		{
			ImGui::Text("Are you sure you want to reset? All unsaved changes will be lost.");
			if (ImGui::Button("Yes")) menu_action = "reset_confirm";
			ImGui::SameLine();
			if (ImGui::Button("No")) menu_action = "";
			ImGui::EndPopup();
		}
		ImGui::OpenPopup("Confirm Reset");
	}

	if (menu_action == "reset_confirm")
	{
		app->inst.deserializeParams(CreateDefaultPreset());
		menu_action = "";
	}

	gui::panel::master("Output", app);
	gui::panel::oscillator("Osc I", &app->inst.osc[0]);
	gui::panel::oscillator("Osc II", &app->inst.osc[1]);
	gui::panel::oscillator("Osc III", &app->inst.osc[2]);
	gui::panel::envelope_adsr("Envelope", &app->inst.env);
	gui::panel::compressor("Compressor", &app->inst.comp);
	gui::panel::resonant_filter("Filter I", &app->inst.filter[0]);
	gui::panel::resonant_filter("Filter II", &app->inst.filter[1]);
	gui::panel::visualizer("Visualizer", app->vis_waveform, app->vis_fft);
	gui::panel::keyboard("Keyboard", &app->keyboard);

	for (int i = 0; i < 4; i++)
		gui::fx::fx_unit(&app->inst, i);

	return true;
}


bool gui::panel::visualizer(const char* label, const ringbuffer<double>& vis_waveform, const visualizer_fft& vis_fft)
{
	if (ImGui::Begin("Visualizer"))
	{
		// calculate available space and halve height
		ImVec2 region = ImGui::GetContentRegionAvail();
		region.y -= region.y * 0.5 + ImGui::GetFrameHeightWithSpacing() * 0.1;

		// lambda helper - required as ImGui does not support doubles
		auto getVal = [](void* data, int idx)
		{
			double* d = (double*)data;
			return (float)d[idx];
		};

		size_t sz_waveform, sz_fft;
		double *p_waveform, *p_fft;
		p_waveform = vis_waveform.dump(sz_waveform);
		p_fft = vis_fft.dump(sz_fft);

		ImGui::PlotLines("Waveform", getVal, p_waveform, sz_waveform, vis_waveform.get_phase(), 0, -1.0f, 1.0f, region);
		ImGui::PlotHistogram("FFT", getVal, p_fft, sz_fft, 0, 0, 0.0f, 64.0f, region);

		ImGui::End();
		return true;
	}

	ImGui::End();
	return false;
}

bool gui::panel::keyboard(const char* label, ::keyboard* board)
{
	if (!ImGui::Begin(label))
	{
		ImGui::End();
		return false;
	}
	
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 region = ImGui::GetWindowSize();
	ImVec2 pos = ImGui::GetWindowPos();

	ImVec2 keysizeWhite = { region.x / (float)board->getWhiteKeys().size(), region.y };
	ImVec2 keysizeBlack = { keysizeWhite.x * 0.7f, keysizeWhite.y * 0.6f };

	auto vKeysWhite = board->getWhiteKeys();
	auto vKeysBlack = board->getBlackKeys();

	// white keys
	for (int i = 0; i < vKeysWhite.size(); i++)
	{
		// bg
		ImVec2 tl = { pos.x + i * keysizeWhite.x, pos.y };
		ImVec2 br = { tl.x + keysizeWhite.x, tl.y + keysizeWhite.y };
		drawList->AddRectFilled(tl, br, 0xFF000000);

		// key
		tl.x += 1.0f; br.x -= 1.0f; br.y -= 1.0f;
		double velocity;
		ImU32 col = 0xFFEEEEEE;
		if (board->keyIsActive(vKeysWhite[i], &velocity))
		{
			// g and b should be inversely proportional to the note velocity
			ImU32 gb = 200 - (ImU32)(velocity * 200.0);
			col = (0xFF << 24) + (gb << 16) + (gb << 8) + 0xFF; // alpha, blue, green, red
		}
		drawList->AddRectFilled(tl, br, col);
	}

	// black keys
	for (int i = 0; i < vKeysBlack.size(); i++)
	{
		int keyId = vKeysBlack.at(i);

		//bg
		int prevKeyIndex = board->getWhiteKeyIndex(keyId - 1);
		ImVec2 tl = { pos.x + prevKeyIndex * keysizeWhite.x + keysizeWhite.x - keysizeBlack.x * 0.5f, pos.y };
		ImVec2 br = { tl.x + keysizeBlack.x, tl.y + keysizeBlack.y };
		drawList->AddRectFilled(tl, br, 0xFF000000);

		// key
		tl.x += 1.0f; br.x -= 1.0f; br.y -= 1.0f;
		double velocity;
		ImU32 col = 0xFF050505;
		if (board->keyIsActive(vKeysBlack[i], &velocity))
		{
			// g and b 0, red proportional to velocity plus offset
			ImU32 uVel = (ImU32)(velocity * 127.0) + (128 - 0x05);
			col = (0xFF << 24) + uVel; // alpha, red
		}
		drawList->AddRectFilled(tl, br, col);
	}

	ImGui::End();
	return true;
}


bool gui::panel::compressor(const char* label, dsp::compressor* comp)
{
	if (comp == nullptr) return false;

	if (ImGui::Begin(label, nullptr, ImGuiWindowFlags_NoCollapse))
	{
		float attack = (float)comp->getAttack();
		float release = (float)comp->getRelease();
		float ratio = (float)comp->getRatio();
		float thresh = (float)comp->getThreshold();
		float ratioLhs = 1.0f / ratio;
		float makeup_dB = (float)comp->getMakeupGain();

		ImGui::Checkbox("Enabled", &comp->enabled); 
		ImGui::SameLine();
		ImGui::Checkbox("Auto Makeup Gain", &comp->autoMakeupGain);

		int flags = ImGuiSliderFlags_AlwaysClamp;
		bool atk = ImGui::SliderFloat("Attack", &attack, 0.1f, 1000.0f, "%.2fms", flags);
		bool rel = ImGui::SliderFloat("Release", &release, 0.1f, 1000.0f, "%.2fms", flags);
		bool rat = ImGui::SliderFloat("Ratio", &ratioLhs, 1.0f, 20.0f, "%.2f:1", flags);
		bool thr = ImGui::SliderFloat("Threshold", &thresh, -48.0f, 0.0f, "%.2fdB", flags);
		bool mkp = ImGui::SliderFloat("Makeup Gain", &makeup_dB, -12.0f, 12.0f, "%.2fdB", flags);

		if (atk) comp->setAttack((double)attack);
		if (rel) comp->setRelease((double)release);
		if (thr) comp->setThreshold((double)thresh);
		if (rat) comp->setRatio(1.0 / (double)ratioLhs);

		if (mkp)
		{
			comp->setMakeupGain((double)makeup_dB);
			//comp->autoMakeupGain = false;
		}

		float gr[1] = { -(float)comp->getGainReduction() };
		ImGui::PlotHistogram("Gain Reduction", &gr[0], 1, 0, nullptr, 0.0, 12.0);

		ImGui::End();
		return true;
	}

	ImGui::End();
	return false;
}


bool gui::panel::resonant_filter(const char* label, dsp::resonant_filter* filter)
{
	if (filter == nullptr) return false;

	if (ImGui::Begin(label))
	{
		int order = filter->GetOrder();
		float cutoff = (float)filter->denormaliseFrequency(filter->GetCutoff());
		float resonance = (float)filter->GetResonance() * 100.0f;

		ImGui::Checkbox("Enabled", &filter->enabled);

		if (ImGui::InputInt("Order", &order, 1, 1, ImGuiInputTextFlags_CharsDecimal))
			filter->SetOrder(order);

		if (ImGui::SliderFloat("Cutoff (Hz)", &cutoff, 20.0f, 18000.0f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_AlwaysClamp))
			filter->SetCutoffHz((double)cutoff);

		if (ImGui::SliderFloat("Resonance", &resonance, 0.00f, 97.0f, "%.2f%%"))
			filter->SetResonance((double)(resonance * 0.01));

		using RFT = resonant_filter::filter_type;
		auto selected = [&](RFT t) { return filter->GetType() == t; };

		if (ImGui::Selectable("HighPass", selected(RFT::HighPass)))
			filter->SetType(RFT::HighPass);

		if (ImGui::Selectable("LowPass", selected(RFT::LowPass)))
			filter->SetType(RFT::LowPass);

		if (ImGui::Selectable("BandPass", selected(RFT::BandPass)))
			filter->SetType(RFT::BandPass);

		if (ImGui::Selectable("BandStop", selected(RFT::BandStop)))
			filter->SetType(RFT::BandStop);

		ImGui::End();
		return true;
	}

	ImGui::End();
	return false;
}


bool gui::panel::oscillator(const char* label, dsp::oscillator* osc)
{
	if (osc == nullptr) return false;

	using wavegen::wave_func;

	if (ImGui::Begin(label))
	{
		float volume_dB = (float)gain::lin2dB(osc->getAmplitude());
		int noteOffset = osc->getNoteOffset();
		int centOffset = osc->getCentOffset();
		int harmonics = osc->getHarmonics();
		float phase = (float)osc->getPhase();

		ImGui::Checkbox("Enabled", &osc->enabled);

		{
			// waveform selection
			const char* labels[] = { "Sine", "Square", "Sawtooth", "Triangle" };
			const wave_func funcs[] = { wave_func::SINE, wave_func::SQUARE, wave_func::SAWTOOTH, wave_func::TRIANGLE };
			int selectedIndex = 0;

			switch (osc->getWaveFunc())
			{
			case wave_func::SINE:     selectedIndex = 0; break;
			case wave_func::SQUARE:   selectedIndex = 1; break;
			case wave_func::SAWTOOTH: selectedIndex = 2; break;
			case wave_func::TRIANGLE: selectedIndex = 3; break;
			}

			if (ImGui::BeginCombo("Shape", labels[selectedIndex]))
			{
				for (int i = 0; i < 4; i++)
				{
					if (ImGui::Selectable(labels[i], i == selectedIndex))
						osc->setWaveFunc(funcs[i]);
					if (i == selectedIndex)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}

		// volume
		if (ImGui::SliderFloat("Volume", &volume_dB, -48.0f, 6.0f, volume_dB > 0.0 ? "+%.2f dB" : "%.2f dB", ImGuiSliderFlags_AlwaysClamp))
			osc->setAmplitude(gain::dB2lin((double)volume_dB));

		// phase
		if (ImGui::SliderFloat("Phase", &phase, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
			osc->setPhase(phase);

		// semitone offset
		if (ImGui::SliderInt("Notes", &noteOffset, -24, 24, "%d", ImGuiSliderFlags_AlwaysClamp))
			osc->setNoteOffset(noteOffset);

		// cent offset
		if (ImGui::SliderInt("Cents", &centOffset, -100, 100, "%d", ImGuiSliderFlags_AlwaysClamp))
			osc->setCentOffset(centOffset);

		// harmonics
		if (osc->getWaveFunc() != wavegen::wave_func::SINE)
			if (ImGui::InputInt("Harmonics", &harmonics, 1, 6, ImGuiInputTextFlags_CharsDecimal))
				osc->setHarmonics(harmonics);

		ImGui::End();
		return true;
	}

	ImGui::End();
	return false;
}


bool gui::panel::envelope_adsr(const char* label, dsp::envelope_adsr* env)
{
	if (env == nullptr) return false;

	if (ImGui::Begin("Envelope"))
	{
		float attack = (float)env->getAttack() * 1000.0f;
		float decay = (float)env->getDecay() * 1000.0f;
		float sustain = (float)env->getSustain() * 100.0f;
		float release = (float)env->getRelease() * 1000.0f;

		bool atk = ImGui::SliderFloat("Attack", &attack, 1.0f, 4000.0f, "%.2fms", ImGuiSliderFlags_Logarithmic);
		bool dec = ImGui::SliderFloat("Decay", &decay, 1.0f, 4000.0f, "%.2fms", ImGuiSliderFlags_Logarithmic);
		bool sus = ImGui::SliderFloat("Sustain", &sustain, 0.0f, 100.0f, "%.2f%%");
		bool rel = ImGui::SliderFloat("Release", &release, 1.0f, 4000.0f, "%.2fms", ImGuiSliderFlags_Logarithmic);

		if (atk) env->setAttack((double)attack * 0.001f);
		if (dec) env->setDecay((double)decay * 0.001f);
		if (sus) env->setSustain((double)sustain * 0.01f);
		if (rel) env->setRelease((double)release * 0.001f);

		ImGui::End();
		return true;
	}

	ImGui::End();
	return false;
}


bool gui::panel::master(const char* label, synth2* app)
{
	if (ImGui::Begin(label))
	{
		int flags = ImGuiSliderFlags_AlwaysClamp;
		float master = (float)app->inst.master_dB;
		float driveIn_dB = (float)app->inst.lim.getInputDrive();
		float lim_gr[] = { -(float)app->inst.lim.getGainReduction_dB() };

		ImGui::InputText("Title", &app->inst.title);		
		bool bDriveIn = ImGui::SliderFloat("Input Drive", &driveIn_dB, -48.0, 12.0, (driveIn_dB > 0.0) ? "+%.2fdB" : "%.2fdB", flags);
		ImGui::PlotHistogram("Limiter GR", &lim_gr[0], 1, 0, 0, 0.0, 12.0);
		bool bMaster = ImGui::SliderFloat("Master", &master, -48.0f, 12.0f, (master > 0.0f) ? "+%2.fdB" : "%.2fdB", flags);

		if (bDriveIn) app->inst.lim.setInputDrive((double)driveIn_dB);
		if (bMaster) app->inst.master_dB = (double)master;

		ImGui::End();
		return true;
	}

	return false;
}


bool gui::fx::fx_unit(instrument* inst, int slot)
{
	bool result = false;
	std::string label = "FX " + std::to_string(slot + 1);

	if (ImGui::Begin(label.c_str()))
	{
		// draw fx unit selector combo
		unit_selection(inst, slot);

		// draw fx UI if assigned
		if (inst->rack[slot] != nullptr)
		{
			if (inst->rack[slot]->getTitle() == "Mono Delay")
				result = mono_delay((dsp::mono_delay*)inst->rack[slot]);
		}
	}

	ImGui::End();
	return result;
}


bool gui::fx::unit_selection(instrument* inst, int slot)
{
	const std::string labels[] = {
		"None",
		"Mono Delay"
	};

	std::string sLabel = "";
	for (int i = 0; i < 2; i++)
		sLabel += labels[i] + '\0';

	int unitId = inst->rack[slot] != nullptr ? inst->rack[slot]->getId() : 0;

	if (ImGui::Combo("FX Unit", &unitId, sLabel.c_str()))
	{
		inst->loadFx(slot, std::string(labels[unitId]), json());
		return true;
	}

	return false;
}


bool gui::fx::mono_delay(dsp::mono_delay* unit)
{
	if (unit == nullptr)
	{
		ImGui::Text("FX Unit is nullptr");
		return false;
	}

	float time = (float)unit->getDelayTime() * 1000.0f;
	float feedback = (float)unit->getFeedback() * 100.0f;
	float mix = (float)unit->getMix() * 100.0f;
	float hi_cutoff = (float)unit->getHiCut();
	float lo_cutoff = (float)unit->getLoCut();
	
	ImGui::Checkbox("Enabled", &unit->enabled);
	bool bTime = ImGui::SliderFloat("Time (ms)", &time, 3.0, unit->MAX_DELAY_TIME * 1000.0f, "%.2fms", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
	bool bFb = ImGui::SliderFloat("Feedback", &feedback, 0.0f, 100.0f, "%.2f%%", ImGuiSliderFlags_AlwaysClamp);
	bool bMix = ImGui::SliderFloat("Mix", &mix, 0.0f, 100.0f, "%.2f%%", ImGuiSliderFlags_AlwaysClamp);
	bool bHPF = ImGui::SliderFloat("Low Cutoff", &lo_cutoff, 20.0, (float)unit->getSampleRate() * 0.3f, "%.2f Hz", ImGuiSliderFlags_Logarithmic);
	bool bLPF = ImGui::SliderFloat("High Cutoff", &hi_cutoff, 200.0, (float)unit->getSampleRate() * 0.5f, "%.2f Hz", ImGuiSliderFlags_Logarithmic);

	if (bTime) unit->setDelayTime((double)time * 0.001);
	if (bFb) unit->setFeedback((double)feedback * 0.01);
	if (bMix) unit->setMix((double)mix * 0.01);
	if (bHPF) unit->setLoCut(lo_cutoff);
	if (bLPF) unit->setHiCut(hi_cutoff);

	return true;
}
