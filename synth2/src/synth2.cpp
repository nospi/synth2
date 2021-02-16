#include "synth2.h"
#include "preset.h"

typedef bool(*lambda)(const note& item);
template<class T>
void safe_remove(T& v, lambda f)
{
	auto n = v.begin();
	while (n != v.end())
		if (!f(*n))
			n = v.erase(n);
		else
			++n;
}

synth2::synth2(synth2::parameters& config) :
	pge_imgui(true), params(config), 
	inst(config.channels, (double)config.sampleRate),
	vis_waveform(config.sampleRate),
	vis_fft(2048)
{
	sAppName = "synth2";
}

bool synth2::OnUserCreate()
{
	gui::initialise();
	initialiseAudio(params.sampleRate, params.channels, params.blocks, params.bufferSize);
	inst.deserializeParams(CreateDefaultPreset());
	return true;
}

bool synth2::OnUserDestroy()
{
	olc::SOUND::DestroyAudio();
	return true;
}

bool synth2::OnUserUpdate(float fElapsedTime)
{
	updateAudio();
	gui::process(this);
	return !(GetKey(olc::ESCAPE).bPressed);
}

bool synth2::initialiseAudio(unsigned sampleRate, unsigned channels, unsigned blocks, unsigned bufferSize)
{
	if (olc::SOUND::InitialiseAudio(sampleRate, channels, blocks, bufferSize))
	{
		olc::SOUND::SetUserSynthFunction(std::bind(&synth2::makeNoise, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return true;
	}
	return false;
}

void synth2::updateAudio()
{
	double timeNow = olc::SOUND::GetTime();
	int noteOffset = 64;

	// check key states to add/remove notes
	for (int k = 0; k < vKeys.size(); k++)
	{
		// Check if note already exists in currently playing notes
		muxNotes.lock();
		auto noteFound = find_if(activeNotes.begin(), activeNotes.end(), [&k, noteOffset](const note& item) { return item.id == k + noteOffset; });
		if (noteFound == activeNotes.end())
		{
			// note not found in vector
			if (GetKey(vKeys[k]).bPressed)
				activeNotes.emplace_back(k + noteOffset, noteOffset, timeNow, 1.0, &inst);
		}
		else
		{
			// note does exist in vector
			if (GetKey(vKeys[k]).bHeld)
			{
				// key still held, do nothing
				if (noteFound->off > noteFound->on)
				{
					// key pressed again during release phase
					noteFound->on = timeNow;
					noteFound->active = true;
				}
			}
		}

		// double check notes outside of the current key ids
		for (auto& n : activeNotes)
			if (!GetKey(vKeys[n.id - n.offset]).bHeld)
				if (n.off < n.on)
					n.off = timeNow;

		muxNotes.unlock();
	}
}

double synth2::makeNoise(int channel, double globalTime, double timeStep)
{
	std::unique_lock<std::mutex> lmNotes(muxNotes);
	double mixedOutput = 0.0;

	for (auto& n : activeNotes)
	{
		bool noteFinished = false;
		double sound = 0.0;
		if (n.channel != nullptr)
			sound = n.channel->sound(globalTime, n, 0, noteFinished);
		mixedOutput += sound;

		if (noteFinished)
			n.active = false;
	}
	safe_remove<std::vector<note>>(activeNotes, [](note const& n) { return n.active; });

	// fx
	for (int i = 0; i < 4; i++)
		if (inst.rack[i] != nullptr && inst.rack[i]->enabled)
			mixedOutput = inst.rack[i]->process(mixedOutput);

	// filters
	for (int i = 0; i < 2; i++)
		if (inst.filter[i].enabled)
			mixedOutput = inst.filter[i].Process(mixedOutput);

	// compressor
	if (inst.comp.enabled)
		mixedOutput = inst.comp.process(mixedOutput);
		
	// limiter
	if (inst.lim.enabled)
		mixedOutput = inst.lim.process(mixedOutput);

	// master fader
	// post-limiter for now; limiter controls to be implemented in future
	mixedOutput *= gain::dB2lin(inst.master_dB);

	// attenuate before output
	mixedOutput *= 0.2;

	// update vis memories
	vis_waveform.write((float)mixedOutput);
	vis_fft.write((float)mixedOutput);

	return mixedOutput;
}

void synth2::makeMuchNoise(double* out, double globalTime, double timeStep)
{
	for (unsigned c = 0; c < params.channels; c++)
		out[c] = 0.0;
}