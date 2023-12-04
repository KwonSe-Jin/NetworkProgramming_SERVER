#include "Motion.h"

GLvoid passiveMotion(int x, int y) {

	hero[global_ID].VAngleMotion(x, y);
	
	glutPostRedisplay();
}