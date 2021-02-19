#pragma once

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <vector>
#include <mutex>
#include <note.h>

class keyboard
{
public:
	keyboard(std::mutex* muxNotes = nullptr, std::vector<dsp::note>* notes = nullptr);
	virtual ~keyboard() = default;

	bool key_is_black(int keyId) const;
	bool key_is_active(int keyId, double* velocity = nullptr) const;

	int getFirstKeyId() const { return startOffset; }
	int getLastKeyId() const { return startOffset + numKeys; }
	int getKeyCount() const { return numKeys; }
	
	std::vector<int> getWhiteKeys() const;
	std::vector<int> getBlackKeys() const;
	int getWhiteKeyIndex(const int& noteId) const;

private:
	int numKeys = 88;
	int startOffset = 21;
	std::mutex* pMuxNotes = nullptr;
	std::vector<dsp::note>* pActiveNotes = nullptr;
};

#endif /* ifndef KEYBOARD_H */