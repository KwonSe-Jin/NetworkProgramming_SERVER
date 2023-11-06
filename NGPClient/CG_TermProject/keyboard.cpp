#include "keyboard.h"
bool isW, isA, isS, isD;
bool isJump;

extern bool RestartPress ;


GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'P':
	case 'p':
		RestartPress = true;
		break;
	case 'w':
	case 'W':
		isW = true;
		break;

	case 'a':
	case 'A':
		isA = true;
		break;
	case 's':
	case 'S':
		isS = true;
		break;
	case 'd':
	case 'D':
		isD = true;
		break;
	case' ':
		isJump = true;
		jumpUp = true;
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;

	case 'r':
	case 'R':
		glutFullScreenToggle();
		break;
	}
	glutPostRedisplay();
}


GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		isA = true;
		break;
	case GLUT_KEY_RIGHT:
		isD = true;
		break;
	case GLUT_KEY_UP:
		isW = true;
		break;
	case GLUT_KEY_DOWN:
		isS = true;
		break;
	case GLUT_KEY_F5:
		glutFullScreenToggle();
		break;
	}

	glutPostRedisplay();
}