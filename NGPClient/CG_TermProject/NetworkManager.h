#pragma once

#include <winSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <queue>
#include <mutex>
using namespace std;
#include "protocol.h"
#include "Header.h"
extern bool isW, isA, isS, isD, isR;
extern bool isBullet;

//#define serverIP "192.168.121.55"
#define serverIP "127.0.0.1"
#define serverPort 9000

#pragma comment(lib, "ws2_32.lib")
void makeInfo(SC_PLAYER_PACKET* p);
void animalInfo(SC_MONSTER_PACKET* p);
void bulletInfo(SC_BULLET_PACKET* p);
void gun_clear();


extern int global_ID;

class NetworkManager {
public:
    NetworkManager();
    

    ~NetworkManager();

    bool Connect();

    bool SendData(const std::string& data);
   
    bool SendPlayerData(CS_PLAYER_PACKET& p);

    bool ReceiveData(std::string& receivedData);

    bool Receiveid();

    bool recvData();
private:
    SOCKET clientSocket;
};