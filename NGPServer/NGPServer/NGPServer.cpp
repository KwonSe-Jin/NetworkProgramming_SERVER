#include "pch.h"

#include "protocol.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "Collision.h"


int HeroID = 0;
vector<Hero> heroes;
vector<SOCKET> clientsocketes;
std::mutex g_m;
queue<CS_PLAYER_PACKET*> playerInput;

mutex player_m;
//전역 만든 것 
int AnimalCnt = 0;

//vector<Hero> heroes; //주인공 벡터 일단 만들어놓음 나중에 맵으로 수정 후 주석 지워주세요. 


bool g_catlive = false;
bool g_doglive = false;
bool g_bearlive = false;
bool g_herodead = false;

float HeroLocationX = 0;
float HeroLocationZ = 0;

int catdead{};
int dogdead{};
int beardead{};

Room catRoom{ Cat };
Room dogRoom{ Dog };
Room bearRoom{ Bear };


vector<Animal*> AniCats{ new Animal(Cat,0), new Animal(Cat,1),new Animal(Cat,2), new Animal(Cat,3),new Animal(Cat,4),new Animal(Cat,5) };
vector<Animal*> AniDogs{ new Animal(Dog,0), new Animal(Dog,1),new Animal(Dog,2), new Animal(Dog,3),new Animal(Dog,4),new Animal(Dog,5) };
Animal AniBear(Bear, 0);
vector<Gun*> gun;

//Attack catattack[AnimalMax];
//Attack dogattack[AnimalMax];
//Attack bearattack;

vector <SC_BULLET_PACKET> bullet;
SC_MONSTER_PACKET monsters[6];
SC_MONSTER_PACKET BossBear;

void SC_PLAYER_Send(SC_PLAYER_PACKET& p, SOCKET clientSocket)
{
    int size = sizeof(p);
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }

}

void SC_MONSTER_Send(SOCKET clientSocket)
{
    int size = sizeof(SC_MONSTER_PACKET) * 6;
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&monsters), sizeof(monsters), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }
}

void SC_BOSSBEAR_Send(SOCKET clientSocket)
{
    int size = sizeof(SC_MONSTER_PACKET);
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&BossBear), sizeof(BossBear), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }
}

void SC_BULLET_Send(SC_BULLET_PACKET& p, SOCKET clientSocket)
{
    int size = sizeof(p);
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }
}

void processCSPlayerPacket(const CS_PLAYER_PACKET& csPacket, SC_PLAYER_PACKET& responsePacket) {

    responsePacket.player_id = csPacket.player_id;
    responsePacket.status = csPacket.status;
    responsePacket.ready = csPacket.ready;
    heroes[responsePacket.player_id].VAngleX = csPacket.camera.VangleX;
    heroes[responsePacket.player_id].VAngleY = csPacket.camera.VangleY;
    /*cout << heroes[scPacket.player_id ].VAngleX << endl;
    cout << heroes[scPacket.player_id ].VAngleY << endl;*/

    if (responsePacket.status) {
		if (csPacket.Player_key.is_w) {
			heroes[responsePacket.player_id].ISW();
		}
		if (csPacket.Player_key.is_a) {
			heroes[responsePacket.player_id].ISA();
		}
		if (csPacket.Player_key.is_s) {
			heroes[responsePacket.player_id].ISS();
		}
		if (csPacket.Player_key.is_d) {
			heroes[responsePacket.player_id].ISD();
		}
		if (csPacket.Player_key.is_q) {
			heroes[csPacket.player_id].isQuit();
		}
        if (csPacket.Player_key.is_bullet) {
            cout << "총 생성" << endl;
            gun.push_back(new Gun{ heroes[responsePacket.player_id].PosX, heroes[responsePacket.player_id].PosY + 0.5f ,heroes[responsePacket.player_id].PosZ
                ,csPacket.Player_key.dirx,csPacket.Player_key.diry,csPacket.Player_key.dirz });
            //cout << csPacket.Player_key.dirx << endl;
        }
    }
}

void Posandlight(SC_PLAYER_PACKET& scPacket, int i)
{
    scPacket.packet_type = SC_PLAYER;
    scPacket.player_id = i;
    scPacket.player_hp = heroes[scPacket.player_id].HP;

    if (!heroes[scPacket.player_id]._flag)
        scPacket.status = false;

    scPacket.Player_pos.x = heroes[scPacket.player_id].PosX;
    scPacket.Player_pos.y = heroes[scPacket.player_id].PosY;
    scPacket.Player_pos.z = heroes[scPacket.player_id].PosZ;

    scPacket.Player_light.R = heroes[scPacket.player_id].lightColorR;
    scPacket.Player_light.G = heroes[scPacket.player_id].lightColorG;
    scPacket.Player_light.B = heroes[scPacket.player_id].lightColorB;

 /*   cout << "플레이어 좌표" << endl;
    cout << scPacket.Player_pos.x << endl;
    cout << scPacket.Player_pos.y << endl;
    cout << scPacket.Player_pos.z << endl;*/
} 


void bulletcalculate(SC_BULLET_PACKET& scPacket, int i)
{
    //BulletCollideBear();
    //BulletCollideDog();
    //BulletCollideCat();
    gun[i]->Update();

    scPacket.packet_type = SC_BULLET;
    scPacket.id = i;

    //p.x = gun[i]->PosX;
    //p.y = gun[i]->PosY;
    //p.z = gun[i]->PosZ;
    scPacket.status = gun[i]->status;
    scPacket.dirx = gun[i]->GunDirX;
    scPacket.diry = gun[i]->GunDirY;
    scPacket.dirz = gun[i]->GunDirZ;
    scPacket.size = gun.size();
    cout << gun.size() << endl;
    //cout << p.packet_type << endl;
}



void processmonsterPacket(Animal& ani) {

    monsters[ani.Index].packet_type = 2;
    monsters[ani.Index].Monster_id = ani.Index;
    if(g_catlive)
        monsters[ani.Index].animal_type = CAT;
    else if (g_doglive)
        monsters[ani.Index].animal_type = DOG;

    monsters[ani.Index].direction = ani.Direction;
    monsters[ani.Index].hp = ani.HP;
  

    ani.update();

	if (g_catlive) {
		AnimalCollideCat();
		Catroomtest();
	}
	if (g_doglive)
	{
		AnimalCollideDog();
		DogAndRoomCollision();
	}
	if (g_bearlive) {
		BearAndRoomCollision();
	}

    monsters[ani.Index].x = ani.PosX;
    monsters[ani.Index].y = ani.PosY;
    monsters[ani.Index].z = ani.PosZ;

}


mutex heroMutex;
//Hero heroInstance;
// 게임 논리를 처리할 계산 스레드
void CalculateThread()
{
    while (true)
    {
        //todo

		g_m.lock();

		for (int i = 0; i < heroes.size(); ++i) {
			if (heroes[i].catlive) { // 한명이라도 들어간 순간? 
				g_catlive = true;
				g_doglive = false;
				g_bearlive = false;
			}
			else if (heroes[i].doglive) {

				g_catlive = false;
				g_doglive = true;
				g_bearlive = false;
			}
			else if (heroes[i].bearlive) {
				g_catlive = false;
				g_doglive = false;
				g_bearlive = true;
			}
		}


		if ((heroes.size()&& g_catlive)|| (heroes.size() && g_doglive)|| (heroes.size() && g_bearlive)) {
            if (g_catlive)
            {
                HeroVSCat();
                for (int i = 0; i < 6; ++i) {
                    processmonsterPacket(*AniCats[i]);
                }
                for (int i = 0; i < heroes.size(); ++i) {

                    SC_MONSTER_Send(clientsocketes[i]);
                }
            }

            if (g_doglive)
            {
                HeroVSDog();
                for (int i = 0; i < 6; ++i) {
                    processmonsterPacket(*AniDogs[i]);
                }
                for (int i = 0; i < heroes.size(); ++i) {

                    SC_MONSTER_Send(clientsocketes[i]);
                }
            }

            if (g_bearlive)
            {
                HeroVSBear();
                BossBear.packet_type = 2;
                BossBear.animal_type = BEAR;

                BossBear.direction = AniBear.Direction;
                BossBear.hp = AniBear.HP;


                AniBear.update();

                BearAndRoomCollision();

                BossBear.x = AniBear.PosX;
                BossBear.y = AniBear.PosY;
                BossBear.z = AniBear.PosZ;
                for (int i = 0; i < heroes.size(); ++i) {
                    SC_BOSSBEAR_Send(clientsocketes[i]);
                }
            }

		}
		
        if(heroes.size() && heroes.size() == HeroID)
        {
            SC_PLAYER_PACKET responsePacket;
            SC_BULLET_PACKET bulletPacket;
            if (!playerInput.empty())
            {
                CS_PLAYER_PACKET* playerInputPacket = playerInput.front();
                playerInput.pop();
                processCSPlayerPacket(*playerInputPacket, responsePacket);

            }


            for (int i = 0; i < heroes.size(); ++i)
            {
                heroes[i].Update();
             
                for (int i = 0; i < gun.size(); ++i)
                {
                    bulletcalculate(bulletPacket, i);
                    for (int j = 0; j < heroes.size(); ++j)
                    {
                        SC_BULLET_Send(bulletPacket, clientsocketes[j]);
                    }
                }
                //클라이언트 입력
                

                Posandlight(responsePacket, i);
                for (int j = 0; j < heroes.size(); ++j) 
                {
                    SC_PLAYER_Send(responsePacket, clientsocketes[j]);
                        
                }

            }


        }
        this_thread::sleep_for(0.5ms);
        g_m.unlock();

    }
}

void HandleClientSocket(SOCKET clientSocket)
{
    //SC_PLAYER_PACKET p;

    //// 클라이언트에게 스레드 ID를 보내기 위한 작업
    //p.player_id = LThreadId;
    //send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);

    // 나머지 클라이언트 소켓 처리 코드

    Hero hero(HeroID); // 스마트 포인터 대신 객체 직접 생성
    {
        lock_guard<mutex> lock(heroMutex);
        heroes.emplace_back(hero); // 직접 객체를 벡터에 추가
    }
    //여기서 ID를 클라에게 보내주기
    SC_PLAYER_PACKET p;

    // 클라이언트에게 스레드 ID를 보내기 위한 작업
    int player_id = HeroID;
    send(clientSocket, reinterpret_cast<char*>(&player_id), sizeof(player_id), 0);
    cout << HeroID << endl;

    ++HeroID;
    //cout << LThreadId << endl;
    //cout << heroes[0].ID << endl;

    char recvBuffer[1000 + 1];
    int recvLen = ::recv(clientSocket, recvBuffer, 1000, 0);
    if (recvLen == SOCKET_ERROR)
    {

        cout << "클라이언트와 연결이 끊김" << endl;
        return;
    }

    recvBuffer[recvLen] = '\0';

    cout << "Recv Data = " << recvBuffer << endl;
    cout << "Recv Data len = " << recvLen << endl;


    while (true)
    {
        //클라이언트에서 키입력 받기 고정길이 // 가변길이 방식

        char buf[100];
        int size;
        recv(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
        //std::cout << size << "바이트 받음" << std::endl;
        int result = recv(clientSocket, buf, size, 0);
        if (result == SOCKET_ERROR)
        {

            cout << "클라이언트와 연결이 끊김" << endl;
            return;
        }
        CS_PLAYER_PACKET* p = reinterpret_cast<CS_PLAYER_PACKET*>(buf);

        lock_guard<mutex> playerLock(player_m);
        playerInput.push(p);
    }
    // 클라이언트 소켓 종료
    SocketUtils::Close(clientSocket);
}

ThreadManager threadManager;
int main()
{
    SocketUtils::Init();

    SOCKET listenSocket = SocketUtils::CreateSocket();
    if (listenSocket == INVALID_SOCKET)
        return 0;

    SocketUtils::SetReuseAddress(listenSocket, true);

    if (SocketUtils::BindAnyAddress(listenSocket, 7777) == false)
        return 0;

    if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
        return 0;

    cout << "서버 대기중.................." << endl;
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    thread calculationThread(CalculateThread);
    //thread MonsterThrea(MonsterThread);

    // Accept
    while (true)
    {
        SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            return 0;
        }

        SocketUtils::SetTcpNoDelay(clientSocket, true);
        
        clientsocketes.emplace_back(clientSocket);
        cout << "Client Connected!" << endl;

        // 클라이언트 별로 스레드 시작
        threadManager.Launch([clientSocket]()
            {
                // 스레드에서 클라이언트 소켓 처리 코드를 실행
                HandleClientSocket(clientSocket);
            });

    }
    threadManager.Join();
    calculationThread.join();
    //MonsterThrea.join();


    SocketUtils::Close(listenSocket);
    SocketUtils::Clear();
}