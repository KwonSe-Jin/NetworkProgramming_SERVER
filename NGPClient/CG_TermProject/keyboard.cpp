#include "keyboard.h"
bool isW, isA, isS, isD, isR;
bool isJump;

extern bool RestartPress ;
extern NetworkManager networkManager;

void get_vangleandStatus(float* x, float* y, bool* status, float* dirx, float* diry, float* dirz);



extern int global_ID;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	CS_PLAYER_PACKET p;
	get_vangleandStatus(&(p.camera.VangleX), &(p.camera.VangleY), &(p.status),
		&(p.Player_key.dirx), &(p.Player_key.diry), &(p.Player_key.dirz));
	p.player_id = global_ID;

	//p.player_id = Th
	switch (key) {
	case 'P':
	case 'p':
		RestartPress = true;
		break;
	case 'w':
	case 'W':
		p.Player_key.is_w = true;
		isW = true;
		break;
	case 'a':
	case 'A':
		p.Player_key.is_a = true;
		isA = true;
		break;
	case 's':
	case 'S':
		p.Player_key.is_s = true;
		isS = true;
		break;
	case 'd':
	case 'D':
		p.Player_key.is_d = true;
		isD = true;
		break;
	case' ':
		p.Player_key.is_space = true;
		isJump = true;
		jumpUp = true;
		break;
	case 'q':
	case 'Q':
		p.Player_key.is_q = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		glutLeaveMainLoop();
		break;
	case 'r':
	case 'R':
		p.ready = true;
		isR = true;
		break;
	/*case 'z':
	case 'Z':
	
		glutFullScreenToggle();
		break;*/

	}
	if (!networkManager.SendPlayerData(p)) {
		std::cout << "패킷보내기 실패" << std::endl;
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