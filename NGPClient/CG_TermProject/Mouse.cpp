#include "Mouse.h"

#include "Sound.h"
extern Sound playSound;
void get_vangle(float* x, float* y);

extern int global_ID;

int BulletLimit;
GLvoid Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!herodead) {
			isBullet = true;
			
			CS_PLAYER_PACKET p;
			get_vangle(&(p.camera.VangleX), &(p.camera.VangleY));
			p.player_id = global_ID;
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