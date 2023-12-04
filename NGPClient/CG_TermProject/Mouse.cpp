#include "Mouse.h"
#include "Sound.h"
extern Sound playSound;
void get_vangleandStatus(float* x, float* y, bool* status, float* dirx, float* diry, float* dirz);

extern NetworkManager networkManager;


extern int global_ID;
//int ball_count = 0;
//int BulletLimit;
//
//bool is_ballfire = false;
//int balltimer = 0;
extern int ball_count;
extern int BulletLimit;
extern bool is_ballfire;
extern int balltimer;
GLvoid Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ball_count < 100) {
		if (!herodead && !balltimer) {
			isBullet = true;
			is_ballfire = true;
			//cout << ball_count << endl;
			CS_PLAYER_PACKET p;
			get_vangleandStatus(&(p.camera.VangleX), &(p.camera.VangleY), &(p.status), 
				&(p.Player_key.dirx), &(p.Player_key.diry), &(p.Player_key.dirz));
			p.player_id = global_ID;
			p.Player_key.is_bullet = true;
			if (p.status) {
				if (!networkManager.SendPlayerData(p)) {
					std::cout << "패킷보내기 실패" << std::endl;
				}
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