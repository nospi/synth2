#pragma once
#include <dsp.h>

using namespace dsp;

class instrument : public instrument_base
{
public:
	double master_dB = 0.0;
	std::string title = "Untitled";
	oscillator osc[3];
	oscillator lfo;
	resonant_filter filter[2];
	envelope_adsr env;
	compressor comp;
	limiter lim;
	fx_unit* rack[4]{ nullptr };

public:
	instrument(unsigned channels = 1, double sampleRate = 44100.0);
	~instrument(); 
	void sound(const double& time, note& n, double* out, bool& noteFinished) override;
	double sound(const double& time, note& n, const int& channel, bool& noteFinished) override;
	json serializeParams() const override;
	bool deserializeParams(const json& j) override;
	bool loadFx(int slot, const std::string& unit, json serializedParams);
};

