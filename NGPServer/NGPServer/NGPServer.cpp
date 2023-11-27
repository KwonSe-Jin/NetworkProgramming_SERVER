#include "pch.h"
#include "Collision.h"
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


bool catlive = false;
bool doglive = false;
bool bearlive = false;
bool herodead = false;

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

Attack catattack[AnimalMax];
Attack dogattack[AnimalMax];
Attack bearattack;

SC_MONSTER_PACKET monsters[6];


void SC_PLAYER_Send(SC_PLAYER_PACKET& p, SOCKET clientSocket)
{

	p.packet_type = SC_PLAYER;
	int size = sizeof(p);
	send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
	int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to send data" << std::endl;
	}

}

void SC_MONSTER_Send(SOCKET clientSocket)
{
	int size = sizeof(SC_MONSTER_PACKET) * 6;
	send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
	int result = send(clientSocket, reinterpret_cast<char*>(&monsters), sizeof(monsters), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to send data" << std::endl;
	}
}

SC_PLAYER_PACKET& processCSPlayerPacket(const CS_PLAYER_PACKET& csPacket) {

	SC_PLAYER_PACKET scPacket;
	scPacket.packet_type = 1;
	scPacket.player_id = csPacket.player_id;

	//heroes[scPacket.player_id].damage();
	scPacket.player_hp = heroes[scPacket.player_id].HP;
	scPacket.status = csPacket.status;
	scPacket.ready = csPacket.ready;
	heroes[scPacket.player_id].VAngleX = csPacket.camera.VangleX;
	heroes[scPacket.player_id].VAngleY = csPacket.camera.VangleY;
	/*cout << heroes[scPacket.player_id ].VAngleX << endl;
	cout << heroes[scPacket.player_id ].VAngleY << endl;*/

	if (csPacket.Player_key.is_w) {

		heroes[scPacket.player_id].ISW();

	}


	if (csPacket.Player_key.is_a) {
		heroes[scPacket.player_id].ISA();
	}

	if (csPacket.Player_key.is_s) {
		heroes[scPacket.player_id].ISS();
	}

	if (csPacket.Player_key.is_d) {
		heroes[scPacket.player_id].ISD();
	}


	scPacket.Player_pos.x = heroes[scPacket.player_id].PosX;
	scPacket.Player_pos.y = heroes[scPacket.player_id].PosY;
	scPacket.Player_pos.z = heroes[scPacket.player_id].PosZ;



	//scPacket.Player_light.R = 1.0f; 
	//scPacket.Player_light.G = 0.5f;
	//scPacket.Player_light.B = 0.0f;


	return scPacket;
}


void processmonsterPacket(Animal& ani) {

	//SC_MONSTER_PACKET scPacket;
	monsters[ani.Index].packet_type = 2;
	monsters[ani.Index].Monster_id = ani.Index;
	monsters[ani.Index].animal_type = ani.AnimalType;
	monsters[ani.Index].direction = ani.Direction;
	//heroes[scPacket.player_id].damage();
	monsters[ani.Index].hp = ani.HP;
	/*cout << heroes[scPacket.player_id ].VAngleX << endl;
	cout << heroes[scPacket.player_id ].VAngleY << endl;*/

	ani.update();

	if (catlive) {
		AnimalCollideCat();
		Catroomtest();
	}
	if (doglive)
	{
		AnimalCollideDog();
		DogAndRoomCollision();
	}
	if (bearlive) {
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
		if (heroes.size()) {
			if (catlive)
				HeroVSCat();

			if (doglive)
				HeroVSDog();

			if (bearlive)
				HeroVSBear();

			for (int i = 0; i < 6; ++i) {
				processmonsterPacket(*AniCats[i]);
			}

			for (int i = 0; i < clientsocketes.size(); ++i) {

				SC_MONSTER_Send(clientsocketes[i]);
			}
		}
		while (!playerInput.empty())
		{
			CS_PLAYER_PACKET* playerInputPacket = playerInput.front();
			playerInput.pop();

			{
				SC_PLAYER_PACKET responsePacket = processCSPlayerPacket(*playerInputPacket);
				heroes[playerInputPacket->player_id].Update();
				/*cout << "playerInputPacket->player_id" << playerInputPacket->player_id << endl;*/

				for (int i = 0; i < clientsocketes.size(); ++i) {
					SC_PLAYER_Send(responsePacket, clientsocketes[i]);
				}

				//to_do 보내기


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


	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}