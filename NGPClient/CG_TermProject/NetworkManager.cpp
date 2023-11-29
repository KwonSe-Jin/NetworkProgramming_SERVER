#include "NetworkManager.h"


NetworkManager::NetworkManager()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed" << std::endl;
		exit(1);
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Failed to create socket" << std::endl;
		WSACleanup();
		exit(1);
	}
}

NetworkManager::~NetworkManager() {
	std::cout << "?????" << std::endl;
	closesocket(clientSocket);
	WSACleanup();
}

bool NetworkManager::Connect() {
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverIP, &serverAddr.sin_addr);

	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cout << "Failed to connect to the server" << std::endl;
		return false;
	}

	return true;
}

bool NetworkManager::SendData(const std::string& data) {
	int result = send(clientSocket, data.c_str(), static_cast<int>(data.size()), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to send data" << std::endl;
		return false;
	}
	return true;
}

bool NetworkManager::SendPlayerData(CS_PLAYER_PACKET& p) {
	// ??????????? ?????? ID?? ?????? ???? ???

	int size = sizeof(p);
	send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
	int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to send data" << std::endl;
		return false;
	}
	return true;
}

bool NetworkManager::ReceiveData(std::string& receivedData) {
	char buffer[1024];
	int result = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to receive data" << std::endl;
		return false;
	}

	if (result > 0) {
		receivedData.assign(buffer, result);
		return true;
	}

	return false; // Connection closed
}

bool NetworkManager::Receiveid() {
	int result = recv(clientSocket, reinterpret_cast<char*>(&global_ID), sizeof(global_ID), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to receive data" << std::endl;
		return false;
	}
	std::cout << global_ID << std::endl;
	return false; // Connection closed
}


bool NetworkManager::recvData() {
	char buf[500];
	int size;
	//std::cout << size << "????? ????" << std::endl;

	recv(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
	recv(clientSocket, buf, size, 0);

	switch (buf[0]) {
	case SC_PLAYER: {
		SC_PLAYER_PACKET* p = reinterpret_cast<SC_PLAYER_PACKET*>(buf);
		makeInfo(p);
	}
				  break;
	case SC_MONSTER: {
		if (size == 32)
		{
			SC_MONSTER_PACKET* p = reinterpret_cast<SC_MONSTER_PACKET*>(buf);
			int i = 0;
			animalInfo(p, i);

		}
		else {
			for (int i = 0; i < 6; ++i) {
				char anibuf[32];
				std::memcpy(&anibuf, &buf[i * 32], 32);
				SC_MONSTER_PACKET* p = reinterpret_cast<SC_MONSTER_PACKET*>(anibuf);
				animalInfo(p, i);

			}
		}
		break;

	}
	case SC_BULLET: {

		std::cout << size << "bullet size" << std::endl;
		SC_BULLET_PACKET* p = reinterpret_cast<SC_BULLET_PACKET*>(buf);

		bulletInfo(p);
		
	}
				  break;
	default:
		std::cout << "error" << endl;
		break;
	}
	return false;
}

