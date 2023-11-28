#include "Timer.h"

extern bool is_ballfire;
extern int balltimer;

void timer(int value) 
{
	if (is_ballfire) {
		++balltimer;
	}

	if (balltimer>=50) {
		is_ballfire = false;
		balltimer = 0;
	}
	if (isW) {
		//hero.ISW();
	}
	else if (isA) {
	
		//hero.ISA();
	}
	else if (isS) {
		
		//hero.ISS();
	}
	else if (isD) {
	
		//hero.ISD();
	}
	if (isJump) {
		//hero.Jump();
	}

	for (int i = 0; i < cats.size(); ++i) {
		cats[i]->update();
	}
	for (int i = 0; i < dogs.size(); ++i) {
		dogs[i]->update();
	}

	if (BearLife) {
		bear.update();
	}

	if (catdead ==6 && !(openingCat >= 1)) {
		openingCat += 0.01;
	}

	if (dogdead == 6 && !(openingDog >= 1)) {
		openingDog += 0.01;
	}

	world.Update();
	glutPostRedisplay();
	glutTimerFunc(13, timer, 0);
}