#pragma once

#include <winSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <iostream>
#include <thread>
#include <queue>
using namespace std;
#include "protocol.h"
#include "Header.h"
extern bool isW, isA, isS, isD;

#define serverIP "127.0.0.1"
#define serverPort 7777

#pragma comment(lib, "ws2_32.lib")
void makeInfo(SC_PLAYER_PACKET* p);


class NetworkManager {
public:
    NetworkManager()
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

    ~NetworkManager() {
        std::cout << "끊김?" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
    }

    bool Connect() {
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

    bool SendData(const std::string& data) {
        int result = send(clientSocket, data.c_str(), static_cast<int>(data.size()), 0);
        if (result == SOCKET_ERROR) {
            std::cout << "Failed to send data" << std::endl;
            return false;
        }
        return true;
    }
    void SendIdlePlayer()
    {
        //idle 상태!
        if (!isW || !isA || !isS || !isD) {
            CS_PLAYER_PACKET p;
            p.Player_key.is_bullet = true;
            if (!SendPlayerData(p)) {
                std::cout << "패킷보내기 실패" << std::endl;
            }
        }
    }
    bool SendPlayerData(CS_PLAYER_PACKET& p) {
        // 클라이언트에게 스레드 ID를 보내기 위한 작업

        int size = sizeof(p);
        send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
        int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
        if (result == SOCKET_ERROR) {
            std::cout << "Failed to send data" << std::endl;
            return false;
        }
        return true;
    }

    bool ReceiveData(std::string& receivedData) {
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

    bool recvData() {
        char buf[100];
        int size;
        recv(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
        std::cout << size << "바이트 받음" << std::endl;
        recv(clientSocket, buf, size, 0);

        switch (buf[0]) {
        case SC_PLAYER: {
            SC_PLAYER_PACKET* p = reinterpret_cast<SC_PLAYER_PACKET*>(buf);
            std::cout << p->Player_pos.x << std::endl;
            std::cout << p->Player_pos.y << std::endl;
            std::cout << p->Player_pos.z << std::endl;
            makeInfo(p);
            std::cout << "들어옴?" << std::endl;
        }
                      break;
        case SC_MONSTER: {
            SC_MONSTER_PACKET* p = reinterpret_cast<SC_MONSTER_PACKET*>(buf);

        }
                       break;
        case SC_BULLET: {
            SC_BULLET_PACKET* p = reinterpret_cast<SC_BULLET_PACKET*>(buf);
        }
                      break;
        default:
            std::cout << "잘못된 데이터" << std::endl;
            break;
        }
        return false;
    }
private:
    SOCKET clientSocket;
};