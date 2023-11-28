#pragma once

#include "Header.h"

class Sound {
	MCI_OPEN_PARMS openGunSound;

public:
	Sound();
	void BasicBGM();
	void stopBGM();
	void GunSound();
};