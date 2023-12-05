#pragma once
#pragma comment(lib, "ws2_32.lib")
#include "TLS.h"
#include "Hero.h"
#include "Attack.h"
#include "Animal.h"
#include "Room.h"
#include "gun.h"
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>

using namespace std;

constexpr float PI = 3.14159265358979323846f;
#define AnimalMax 6

#define Cat 1
#define Dog 2
#define Bear 3



extern vector<Hero> heroes; 


extern int AnimalCnt ;


extern bool g_catlive;
extern bool g_doglive;
extern bool g_bearlive;
extern bool g_herodead;



extern float HeroLocationX;
extern float HeroLocationZ;



extern vector<Animal*> AniCats;
extern vector<Animal*> AniDogs;
extern Animal AniBear;
extern vector<Gun*> gun;

extern int readycount;


extern Room catRoom;
extern Room dogRoom;
extern Room bearRoom;
extern int RestartCnt;

