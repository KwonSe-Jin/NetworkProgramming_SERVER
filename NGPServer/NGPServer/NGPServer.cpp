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
queue<CS_PLAYER_PACKET*> playerInput1;
queue<CS_PLAYER_PACKET*> playerInput2;
queue<SC_PLAYER_PACKET> playerqueue;
queue<SC_MONSTER_PACKET> monsterqueue;
queue<SC_BULLET_PACKET> bulletqueue;

mutex player_m;
mutex player_m1;
mutex player_m2;
mutex player_m3;
mutex player_m4;
mutex player_m5;
mutex player_m6;
mutex player_m7;
mutex player_m8;
mutex player_m9;
mutex send_m;
//전역 만든 것 
int AnimalCnt = 0;
extern int readycount = 0;
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

bool toggle1 = true;
bool toggle2 = true;

void SC_PLAYER_Send(SC_PLAYER_PACKET& p, SOCKET clientSocket)
{
    p.packet_type = SC_PLAYER;
    int size = sizeof(p);
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }

}

void SC_MONSTER_Send(SC_MONSTER_PACKET& p, SOCKET clientSocket)
{
    p.packet_type = SC_MONSTER;
    int size = sizeof(p);
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }
}

void SC_BULLET_Send(SC_BULLET_PACKET& p, SOCKET clientSocket)
{
    p.packet_type = SC_BULLET;
    int size = sizeof(p);
    send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
    int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
    if (result == SOCKET_ERROR) {
        //std::cout << "Failed to send data" << std::endl;
    }
}

void processCSPlayerPacket(const CS_PLAYER_PACKET& csPacket)
{


    heroes[csPacket.player_id].VAngleX = csPacket.camera.VangleX;
    heroes[csPacket.player_id].VAngleY = csPacket.camera.VangleY;
    /*cout << heroes[scPacket.player_id ].VAngleX << endl;
    cout << heroes[scPacket.player_id ].VAngleY << endl;*/

    if (csPacket.status) {
        if (csPacket.Player_key.is_w) {
            heroes[csPacket.player_id].ISW();
        }
        if (csPacket.Player_key.is_a) {
            heroes[csPacket.player_id].ISA();
        }
        if (csPacket.Player_key.is_s) {
            heroes[csPacket.player_id].ISS();
        }
        if (csPacket.Player_key.is_d) {
            heroes[csPacket.player_id].ISD();
        }
        if (csPacket.Player_key.is_q) {
            heroes[csPacket.player_id].isQuit();
        }
        if (csPacket.Player_key.is_bullet) {
            //cout << "총 생성" << endl;
            gun.push_back(new Gun{ heroes[csPacket.player_id].PosX, heroes[csPacket.player_id].PosY + 0.5f ,heroes[csPacket.player_id].PosZ
               ,csPacket.Player_key.dirx,csPacket.Player_key.diry,csPacket.Player_key.dirz });
            //cout << csPacket.Player_key.dirx << endl;
        }
        if (csPacket.ready) {
            if (heroes[csPacket.player_id]._readyflag == false)
                heroes[csPacket.player_id].ISR();
            cout << readycount << endl;
        }
        if (csPacket.Player_key.is_q) {
            heroes[csPacket.player_id].isQuit();
        }
    }
}

void Posandlight(SC_PLAYER_PACKET& scPacket, int i)
{
    scPacket.packet_type = SC_PLAYER;
    scPacket.player_id = i;
    scPacket.player_hp = heroes[scPacket.player_id].HP;

    if (heroes[scPacket.player_id]._flag)
        scPacket.status = true;
    else {
        scPacket.status = false;
    }

    scPacket.Player_pos.x = heroes[scPacket.player_id].PosX;
    scPacket.Player_pos.y = heroes[scPacket.player_id].PosY;
    scPacket.Player_pos.z = heroes[scPacket.player_id].PosZ;

    scPacket.Player_light.R = heroes[scPacket.player_id].lightColorR;
    scPacket.Player_light.G = heroes[scPacket.player_id].lightColorG;
    scPacket.Player_light.B = heroes[scPacket.player_id].lightColorB;

    scPacket.ready = heroes[scPacket.player_id]._readyflag;

}



void bulletcalculate(SC_BULLET_PACKET& scPacket, int i)
{
    BulletCollideBear();
    BulletCollideDog();
    BulletCollideCat();
    gun[i]->Update();

    scPacket.packet_type = SC_BULLET;
    scPacket.id = i;


    scPacket.status = gun[i]->status;
    scPacket.dirx = gun[i]->GunDirX;
    scPacket.diry = gun[i]->GunDirY;
    scPacket.dirz = gun[i]->GunDirZ;
    scPacket.size = gun.size();
}



void processmonsterPacket(SC_MONSTER_PACKET& monster, int i) {



    if (g_catlive)
    {
        monster.animal_type = CAT;
        monster.direction = AniCats[i]->Direction;
        monster.hp = AniCats[i]->HP;

    }
    else if (g_doglive)
    {
        monster.animal_type = DOG;
        monster.direction = AniDogs[i]->Direction;
        monster.hp = AniDogs[i]->HP;

    }

    int d_Cat = 0;
    int d_Dog = 0;

    if (g_catlive) {
        AnimalCollideCat();
        Catroomtest();
        for (int i = 0; i < AniCats.size(); ++i) {
            if (AniCats[i]->HP <= 0)
                ++d_Cat;
        }

        if (d_Cat >= 6) {

        }
        else {
            HeroVSCat();
        }
    }
    if (g_doglive)
    {
        AnimalCollideDog();
        DogAndRoomCollision();

        for (int i = 0; i < AniDogs.size(); ++i) {
            if (AniDogs[i]->HP <= 0)
                ++d_Dog;
        }
        if (d_Dog >= 6) {
        }
        else {
            HeroVSDog();
        }
    }
    if (g_bearlive) {
        HeroVSBear();

        BearAndRoomCollision();
    }

    monster.packet_type = 2;
    monster.Monster_id = i;
    if (g_catlive)
    {
        monster.animal_type = CAT;
        monster.direction = AniCats[i]->Direction;
        monster.hp = AniCats[i]->HP;
        AniCats[i]->update();
        monster.x = AniCats[i]->PosX;
        monster.y = AniCats[i]->PosY;
        monster.z = AniCats[i]->PosZ;
    }
    else if (g_doglive)
    {
        monster.animal_type = DOG;
        monster.direction = AniDogs[i]->Direction;
        monster.hp = AniDogs[i]->HP;
        AniDogs[i]->update();
        monster.x = AniDogs[i]->PosX;
        monster.y = AniDogs[i]->PosY;
        monster.z = AniDogs[i]->PosZ;
    }
}

void SendQueue()
{
    while (true)
    {
        send_m.lock();


        if (heroes.size()) {
            {
                if (!playerqueue.empty())
                {
                    player_m1.lock();

                    SC_PLAYER_PACKET p = playerqueue.front();
                    playerqueue.pop();
                    for (int j = 0; j < heroes.size(); ++j)
                    {
                        if (!heroes[j].is_q && heroes[j].toggleID == false) {
                            SC_PLAYER_Send(p, clientsocketes[j]);
                        }
                    }
                    player_m1.unlock();

                }

            }

            {

                if (!bulletqueue.empty())
                {
                    player_m2.lock();

                    SC_BULLET_PACKET p = bulletqueue.front();
                    bulletqueue.pop();
                    for (int j = 0; j < heroes.size(); ++j)
                    {
                        if (!heroes[j].is_q && heroes[j].toggleID == false)
                            SC_BULLET_Send(p, clientsocketes[j]);
                    }
                    player_m2.unlock();

                }

            }
            {
                if (!monsterqueue.empty())
                {
                    player_m3.lock();

                    SC_MONSTER_PACKET p = monsterqueue.front();
                    monsterqueue.pop();
                    for (int j = 0; j < heroes.size(); ++j)
                    {
                        if (!heroes[j].is_q && heroes[j].toggleID == false)
                            SC_MONSTER_Send(p, clientsocketes[j]);
                    }
                    player_m3.unlock();

                }
            }

        }
        send_m.unlock();

    }

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
                if (toggle1) {
                    gun.erase(gun.begin(), gun.end());
                    cout << " gun.size() == " << gun.size() << endl;
                    toggle1 = !toggle1;
                }

            }
            else if (heroes[i].bearlive) {
                g_catlive = false;
                g_doglive = false;
                g_bearlive = true;
                if (toggle2) {
                    gun.erase(gun.begin(), gun.end());
                    cout << " gun.size() == " << gun.size() << endl;

                    toggle2 = !toggle2;
                }

            }
        }
        if (heroes.size() && heroes.size() == HeroID)
        {
            SC_PLAYER_PACKET responsePacket;
            SC_BULLET_PACKET bulletPacket;
            player_m4.lock();
            {
                if (!playerInput.empty()) {
                    CS_PLAYER_PACKET* playerInputPacket = playerInput.front();
                    playerInput.pop();
                    processCSPlayerPacket(*playerInputPacket);
                }
            }
            player_m4.unlock();

            /*player_m1.lock();
            if (!playerInput1.empty()) {
               CS_PLAYER_PACKET* playerInputPacket = playerInput.front();
               playerInput.pop();
               processCSPlayerPacket(*playerInputPacket);
            }
            player_m1.unlock();
            player_m2.lock();

            if (!playerInput2.empty()) {
               CS_PLAYER_PACKET* playerInputPacket = playerInput.front();
               playerInput.pop();
               processCSPlayerPacket(*playerInputPacket);
            }
            player_m2.unlock();*/



            for (int i = 0; i < heroes.size(); ++i)
            {
                if (!heroes[i].is_q) {
                    heroes[i].Update();
                }

                Posandlight(responsePacket, i);
                {
                    player_m8.lock();

                    playerqueue.push(responsePacket);
                    player_m8.unlock();

                }



            }


            for (int i = 0; i < gun.size(); ++i)
            {
                bulletcalculate(bulletPacket, i);
                {
                    player_m9.lock();

                    bulletqueue.push(bulletPacket);
                    player_m9.unlock();

                }



            }

        }

        if ((heroes.size() && g_catlive) || (heroes.size() && g_doglive) || (heroes.size() && g_bearlive)) {
            if (g_catlive)
            {
                SC_MONSTER_PACKET monster;
                player_m5.lock();

                for (int i = 0; i < 6; ++i)
                {
                    processmonsterPacket(monster, i);
                    monsterqueue.push(monster);


                }
                player_m5.unlock();

            }

            if (g_doglive)
            {
                SC_MONSTER_PACKET monster;
                player_m6.lock();

                for (int i = 0; i < 6; ++i)
                {
                    processmonsterPacket(monster, i);
                    monsterqueue.push(monster);


                }
                player_m6.unlock();

            }

            if (g_bearlive)
            {
                SC_MONSTER_PACKET BossBear;
                player_m7.lock();

                BossBear.packet_type = 2;
                BossBear.animal_type = BEAR;


                BossBear.direction = AniBear.Direction;
                BossBear.hp = AniBear.HP;
                HeroVSBear();

                BearAndRoomCollision();

                AniBear.update();


                BossBear.x = AniBear.PosX;
                BossBear.y = AniBear.PosY;
                BossBear.z = AniBear.PosZ;
                monsterqueue.push(BossBear);
                player_m7.unlock();



            }

        }


        this_thread::sleep_for(0.1ms);

        g_m.unlock();
    }


}
std::mutex heroIdMutex;
void HandleClientSocket(SOCKET clientSocket)
{

    {
        std::lock_guard<std::mutex> lock(heroIdMutex);
        Hero hero(HeroID); // 스마트 포인터 대신 객체 직접 생성
        {
            lock_guard<mutex> lock(heroMutex);
            heroes.emplace_back(hero); // 직접 객체를 벡터에 추가
        }
        //여기서 ID를 클라에게 보내주기

        // 클라이언트에게 스레드 ID를 보내기 위한 작업


        if (heroes[HeroID].ID != -1 && heroes[HeroID].toggleID) { //id할당받고, 보내지 않았음 
            send(clientsocketes[HeroID], reinterpret_cast<char*>(&heroes[HeroID].ID), sizeof(heroes[HeroID].ID), 0);
            heroes[HeroID].toggleID = false; // 바로 토글끄기 
        }
        ++HeroID;


    }
    //++HeroID;
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
        recv(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
        //std::cout << size << "바이트 받음" << std::endl;
        int result = recv(clientSocket, buf, size, MSG_WAITALL);
        if (result == SOCKET_ERROR)
        {

            cout << "클라이언트와 연결이 끊김" << endl;
            return;
        }
        CS_PLAYER_PACKET* p = reinterpret_cast<CS_PLAYER_PACKET*>(buf);
        cout << p->player_id << endl;
        {
            player_m.lock();
            //if (p->player_id == 0)
            playerInput.push(p);
            player_m.unlock();

        }
        /*{
           lock_guard<mutex> playerLock(player_m1);

           if (p->player_id == 1)
              playerInput1.push(p);
        }
        {
           lock_guard<mutex> playerLock(player_m2);
           if (p->player_id == 2)
              playerInput2.push(p);
        }*/


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

    if (SocketUtils::BindAnyAddress(listenSocket, 9000) == false)
        return 0;

    if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
        return 0;

    cout << "서버 대기중.................." << endl;
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    thread calculationThread(CalculateThread);
    thread SendThread(SendQueue);
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
    SendThread.join();

    SocketUtils::Close(listenSocket);
    SocketUtils::Clear();
}