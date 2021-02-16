#include "preset.h"
#include <dsp.h>
#include <fstream>
#include <iomanip>

using namespace dsp;

// TODO: status/error responses

bool ReadJSONFromFile(const std::string& filename, json& j)
{
	std::ifstream i(filename);
	if (!i.is_open() || !i.good()) return false;
	i >> j;
	return true;
}

bool WriteJSONToFile(const std::string& filename, const json& j)
{
	std::ofstream o(filename, std::ios_base::trunc);
	if (!o.is_open() || !o.good())
	{
		return false;
	}
	o << std::setw(4) << j << std::endl;
	return true;
}

json CreateDefaultPreset()
{
	json j;

	j["title"] = "Untitled";
	j["master_dB"] = 0.0;

	oscillator osc;
	j["osc"][0] = osc.serializeParams();
	j["osc"][1] = osc.serializeParams();
	j["osc"][2] = osc.serializeParams();
	j["osc"][0]["enabled"] = true;

	oscillator lfo(0.0, 0.0, wavegen::SINE, 1);
	j["lfo"] = lfo.serializeParams();

	resonant_filter filt;
	j["filter"][0] = filt.serializeParams();
	j["filter"][1] = filt.serializeParams();

	envelope_adsr env;
	j["env"] = env.serializeParams();

	compressor comp;
	j["comp"] = comp.serializeParams();

	j["fx"][0] = { { "unit", "None" }, { "params", nullptr } };
	j["fx"][1] = { { "unit", "None" }, { "params", nullptr } };
	j["fx"][2] = { { "unit", "None" }, { "params", nullptr } };
	j["fx"][3] = { { "unit", "None" }, { "params", nullptr } };

	return j;
}