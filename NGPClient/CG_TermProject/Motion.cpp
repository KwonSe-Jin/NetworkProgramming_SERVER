#include "Motion.h"

GLvoid passiveMotion(int x, int y) {

	/*CS_PLAYER_PACKET p;
	p.Player_key.is_bullet = true;
	if (!networkManager.SendPlayerData(p)) {
		std::cout << "��Ŷ������ ����" << std::endl;
	}*/
	hero[global_ID].VAngleMotion(x, y);
	
	glutPostRedisplay();
}