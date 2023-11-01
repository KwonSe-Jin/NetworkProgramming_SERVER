#include "Motion.h"

GLvoid passiveMotion(int x, int y) {

	
	hero.VAngleMotion(x, y);
	
	//glutPostRedisplay();
}