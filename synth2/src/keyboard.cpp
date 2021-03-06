#include "keyboard.h"

keyboard::keyboard(std::mutex* muxNotes, std::vector<dsp::note>* notes) :
	pMuxNotes(muxNotes), pActiveNotes(notes)
{}

bool keyboard::keyIsBlack(int keyId) const
{
	keyId %= 12;
	return (keyId + keyId / 5 + keyId / 10) % 2;
}

bool keyboard::keyIsActive(int keyId, double* velocity)
{
	auto keyFound = std::find_if(pActiveNotes->begin(), pActiveNotes->end(), [&keyId](const dsp::note& n) { return n.id == keyId; });
	if (keyFound != pActiveNotes->end())
	{
		if (velocity != nullptr)
			*velocity = keyFound->velocity;
		return keyFound->active && keyFound->off < keyFound->on;
	}
	return false;
}

std::vector<int> keyboard::getWhiteKeys()
{
	static std::vector<int> keys;
	if (keys.size() < 1)
		for (int i = startOffset; i < startOffset + numKeys; i++)
			if (!keyIsBlack(i))
				keys.push_back(i);
	return keys;
}

std::vector<int> keyboard::getBlackKeys()
{
	static std::vector<int> keys;
	if (keys.size() < 1)
		for (int i = startOffset; i < startOffset + numKeys; i++)
			if (keyIsBlack(i))
				keys.push_back(i);
	return keys;
}

int keyboard::getWhiteKeyIndex(const int& noteId)
{
	auto keys = getWhiteKeys();
	for (int i = 0; i < keys.size(); i++)
		if (keys[i] == noteId)
			return i;
	return -1;
}
