#include "Motion.h"

GLvoid passiveMotion(int x, int y) {

	/*CS_PLAYER_PACKET p;
	p.Player_key.is_bullet = true;
	if (!networkManager.SendPlayerData(p)) {
		std::cout << "패킷보내기 실패" << std::endl;
	}*/
	hero[global_ID].VAngleMotion(x, y);
	
	glutPostRedisplay();
}