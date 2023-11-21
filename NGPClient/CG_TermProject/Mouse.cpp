#include "Mouse.h"

#include "Sound.h"
extern Sound playSound;

int BulletLimit;
GLvoid Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!herodead) {
			isBullet = true;
			
			CS_PLAYER_PACKET p;
			p.Player_key.is_bullet = true;
			if (!networkManager.SendPlayerData(p)) {
				std::cout << "패킷보내기 실패" << std::endl;
			}

			playSound.GunSound();
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		BulletLimit = 0;
		isBullet = false;
	}
}