#include "Timer.h"

void timer(int value) 
{

	if (isW) {
		hero.ISW();
	}
	else if (isA) {
	
		hero.ISA();
	}
	else if (isS) {
		
		hero.ISS();
	}
	else if (isD) {
	
		hero.ISD();
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
	glutTimerFunc(14, timer, 1);
}