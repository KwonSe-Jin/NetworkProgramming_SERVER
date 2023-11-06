#include "Mouse.h"

#include "Sound.h"
extern Sound playSound;

int BulletLimit;
GLvoid Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!herodead) {
			isBullet = true;

			playSound.GunSound();
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		BulletLimit = 0;
		isBullet = false;
	}
}