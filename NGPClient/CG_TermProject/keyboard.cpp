#include "keyboard.h"
bool isW, isA, isS, isD;
bool isJump;

extern bool RestartPress ;
extern NetworkManager networkManager;


GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'P':
	case 'p':
		RestartPress = true;
		break;
	case 'w':
	case 'W':
	{
		CS_PLAYER_PACKET p;
		p.Player_key.is_w = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		isW = true;
		break;
	}
	case 'a':
	case 'A':
	{
		CS_PLAYER_PACKET p;
		p.Player_key.is_a = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		isA = true;
		break;
	}
	case 's':
	case 'S':
	{
		CS_PLAYER_PACKET p;
		p.Player_key.is_s = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		isS = true;
		break;
	}
	case 'd':
	case 'D':
	{
		CS_PLAYER_PACKET p;
		p.Player_key.is_d = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		isD = true;
		break;
	}
	case' ':
	{
		CS_PLAYER_PACKET p;
		p.Player_key.is_space = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		isJump = true;
		jumpUp = true;
		break;
	}
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;

	case 'r':
	case 'R':
	{
		CS_PLAYER_PACKET p;
		p.ready = true;
		if (!networkManager.SendPlayerData(p)) {
			std::cout << "패킷보내기 실패" << std::endl;
		}
		break;
	}
	case 'z':
	case 'Z':
	
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