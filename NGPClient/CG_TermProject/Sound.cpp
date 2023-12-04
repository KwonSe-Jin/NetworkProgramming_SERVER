#include "sound.h"

Sound::Sound()
{
	openGunSound.lpstrElementName = "최종총소리.wav";
	openGunSound.lpstrDeviceType = "waveaudio";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)(LPMCI_OPEN_PARMS)&openGunSound);
}

void Sound::BasicBGM()
{
	PlaySound("전투시작.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void Sound::stopBGM()
{
	PlaySound(NULL, NULL, SND_PURGE | SND_NOWAIT | SND_ASYNC);
}

void Sound::GunSound()
{
	mciSendCommand(openGunSound.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)(LPVOID)NULL);    //음원 재생 위치를 처음으로 초기화
	mciSendCommand(openGunSound.wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)(LPMCI_PLAY_PARMS)NULL);    //음악 반복 재생
}
