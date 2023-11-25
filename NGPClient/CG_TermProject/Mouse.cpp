#include "Mouse.h"
#include "Sound.h"
extern Sound playSound;
void get_vangle(float* x, float* y);


extern int global_ID;
int ball_count = 0;
int BulletLimit;

bool is_ballfire = false;
int balltimer = 0;
GLvoid Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ball_count < 100) {
		if (!herodead && !balltimer) {
			isBullet = true;
			is_ballfire = true;
			cout << ball_count << endl;
			CS_PLAYER_PACKET p;
			get_vangle(&(p.camera.VangleX), &(p.camera.VangleY));
			p.player_id = global_ID;
			p.Player_key.is_bullet = true;
			if (!networkManager.SendPlayerData(p)) {
				std::cout << "��Ŷ������ ����" << std::endl;
			}
			ball_count++;
			playSound.GunSound();
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		BulletLimit = 0;
		isBullet = false;
	}
}