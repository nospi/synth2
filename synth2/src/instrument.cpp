#include "instrument.h"

instrument::instrument(unsigned channels, double sampleRate) :
	instrument_base(channels, sampleRate), master_dB(0.0),
	lim(sampleRate), comp(sampleRate)
{
	for (int i = 0; i < 2; i++)
		filter[i] = resonant_filter(sampleRate);
}

instrument::~instrument()
{
	for (int i = 0; i < 4; i++)
		if (rack[i] != nullptr)
			delete rack[i];
}

void instrument::sound(const double& time, note& n, double* out, bool& noteFinished)
{
	for (int c = 0; c < getChannels(); c++)
		out[c] = sound(time, n, c, noteFinished);
}

double instrument::sound(const double& time, note& n, const int& channel, bool& noteFinished)
{
	double sound = 0.0;
	double amplitude = 0.0;

	amplitude = n.env.process(time, n.on, n.off, n.velocity);
	if (amplitude <= 0.0 && n.off > n.on) noteFinished = true;

	for (int i = 0; i < 3; i++)
		if (osc[i].enabled)
			sound += osc[i].run(n, time) * amplitude;
	
	return sound;
}

json instrument::serializeParams() const
{
	json j = {
		{ "title", title },
		{ "master_dB", master_dB },
		{ "osc", {
			osc[0].serializeParams(),
			osc[1].serializeParams(),
			osc[2].serializeParams()
		}},
		{ "lfo", lfo.serializeParams() },
		{ "env", env.serializeParams() },
		{ "comp", comp.serializeParams() },
		{ "filter", {
			filter[0].serializeParams(),
			filter[1].serializeParams()
		}}
	};

	// handle FX units
	for (int i = 0; i < 4; i++)
	{
		json jfx = {
			{ "unit", "None" },
			{ "params", {} }
		};
		if (rack[i] != nullptr)
		{
			jfx["unit"] = rack[i]->getTitle();
			jfx["params"] = rack[i]->serializeParams();
		}
		j["fx"][i] = jfx;
	}

	return j;
}

bool instrument::deserializeParams(const json& j)
{
	try
	{
		title = j["title"];
		master_dB = j["master_dB"];
		osc[0].deserializeParams(j["osc"][0]);
		osc[1].deserializeParams(j["osc"][1]);
		osc[2].deserializeParams(j["osc"][2]);
		lfo.deserializeParams(j["lfo"]);
		env.deserializeParams(j["env"]);
		comp.deserializeParams(j["comp"]);
		filter[0].deserializeParams(j["filter"][0]);
		filter[1].deserializeParams(j["filter"][1]);
		loadFx(0, j["fx"][0]["unit"], j["fx"][0]["params"]);
		loadFx(1, j["fx"][1]["unit"], j["fx"][1]["params"]);
		loadFx(2, j["fx"][2]["unit"], j["fx"][2]["params"]);
		loadFx(3, j["fx"][3]["unit"], j["fx"][3]["params"]);
		return true;
	}
#pragma warning(push)
#pragma warning(disable: 4101)	// unused variable _e
	catch (std::exception& _e)
#pragma warning(pop)
	{
		return false;
	}
}

bool instrument::loadFx(int slot, const std::string& unit, json serializedParams)
{
	// we're limited to 4 fx units by design
	if (slot < 0 || slot >= 4)
		return false;

	// take care of memory de/alloc
	auto clearSlot = [&](int slot)
	{
		if (rack[slot] != nullptr)
		{
			delete rack[slot];
			rack[slot] = nullptr;
		}
	};

	// empty rack unit
	if (unit == "None" && rack[slot] != nullptr && rack[slot]->getTitle() != "None")
	{
		clearSlot(slot);
		return true;
	}

	// mono delay
	if (unit == "Mono Delay")
	{
		clearSlot(slot);
		rack[slot] = new mono_delay(0.5, 0.33, 44100.0);
	}

	// load params if provided
	if (serializedParams != nullptr)
		rack[slot]->deserializeParams(serializedParams);

	return true;
}
