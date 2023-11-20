#include "pch.h"

#include "protocol.h"
#include "ThreadManager.h"
#include "SocketUtils.h"



vector<Hero> heroes;
std::mutex g_m;
using PlayerInputTuple = std::tuple<CS_PLAYER_PACKET*, Hero, SOCKET>;
std::queue<PlayerInputTuple> playerInput;

mutex player_m;
//전역 만든 것 
int AnimalCnt = 0;

//vector<Hero> heroes; //주인공 벡터 일단 만들어놓음 나중에 맵으로 수정 후 주석 지워주세요. 


 bool catlive=false;
 bool doglive = false;
 bool bearlive = false;
 bool herodead = false;

 float HeroLocationX=0;
 float HeroLocationZ=0;


 int catdead{};
 int dogdead{};
 int beardead{};

 Room catRoom{ Cat };
 Room dogRoom{ Dog };
 Room bearRoom{ Bear };


 vector<Animal*> AniCats{ new Animal(Cat,0), new Animal(Cat,1),new Animal(Cat,2), new Animal(Cat,3),new Animal(Cat,4),new Animal(Cat,5) };
 vector<Animal*> AniDogs{ new Animal(Dog,0), new Animal(Dog,1),new Animal(Dog,2), new Animal(Dog,3),new Animal(Dog,4),new Animal(Dog,5) };
 Animal AniBear(Bear,0);


Attack catattack[AnimalMax];
Attack dogattack[AnimalMax];
Attack bearattack;


void SendToClient(SC_PLAYER_PACKET& p, SOCKET clientSocket)
{
	//SC_PLAYER_PACKET p;
	p.packet_type = SC_PLAYER;
	int size = sizeof(p);
	send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
	int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to send data" << std::endl;

	}
}

SC_PLAYER_PACKET processCSPlayerPacket(const CS_PLAYER_PACKET& csPacket, Hero& hero) {

	SC_PLAYER_PACKET scPacket;
	scPacket.packet_type = 1; 
	scPacket.player_id = csPacket.player_id;
	hero.damage();
	scPacket.player_hp = hero.HP;
	scPacket.status = csPacket.status;
	scPacket.ready = csPacket.ready;

	if (csPacket.Player_key.is_w) {
		hero.ISW();
	}

	if (csPacket.Player_key.is_a) {
		hero.ISA();
	}

	if (csPacket.Player_key.is_s) {
		hero.ISS();
	}

	if (csPacket.Player_key.is_d) {
		hero.ISD();
	}

	
	scPacket.Player_pos.x = hero.PosX;
	scPacket.Player_pos.y = hero.PosY; 
	scPacket.Player_pos.z = hero.PosZ;


	//scPacket.Player_light.R = 1.0f; 
	//scPacket.Player_light.G = 0.5f;
	//scPacket.Player_light.B = 0.0f;

	return scPacket;
}

mutex heroMutex;
//Hero heroInstance;
// 게임 논리를 처리할 계산 스레드
void CalculateThread()
{
	while (true)
	{
		//todo

		player_m.lock();
		while (!playerInput.empty())
		{
			std::tuple<CS_PLAYER_PACKET*, Hero, SOCKET> packetInfo = playerInput.front();
			playerInput.pop();

			CS_PLAYER_PACKET* playerInputPacket = std::get<0>(packetInfo);
			Hero heroRef = std::get<1>(packetInfo);
			SOCKET clientSocket = std::get<2>(packetInfo);

			{
				lock_guard<mutex> lock(heroMutex);
				SC_PLAYER_PACKET responsePacket = processCSPlayerPacket(*playerInputPacket, heroRef);
				SendToClient(responsePacket, clientSocket);
			}

			//SendToClient(responsePacket, clientSocket);
			delete playerInputPacket;
		}
		player_m.unlock();
	}
}

void HandleClientSocket(SOCKET clientSocket)
{
	//SC_PLAYER_PACKET p;

	//// 클라이언트에게 스레드 ID를 보내기 위한 작업
	//p.player_id = LThreadId;
	//send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);

	// 나머지 클라이언트 소켓 처리 코드

	Hero hero(LThreadId); // 스마트 포인터 대신 객체 직접 생성
	{
		lock_guard<mutex> lock(heroMutex);
		heroes.emplace_back(hero); // 직접 객체를 벡터에 추가
	}


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

	SC_PLAYER_PACKET p;
	p.packet_type = SC_PLAYER;
	int size = sizeof(p);
	send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
	int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
	if (result == SOCKET_ERROR) {
		std::cout << "Failed to send data" << std::endl;
	}

	while (true)
	{
		//클라이언트에서 키입력 받기 고정길이 // 가변길이 방식

		char buf[100];
		int size;
		recv(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
		std::cout << size << "바이트 받음" << std::endl;
		int result = recv(clientSocket, buf, size, MSG_WAITALL);
		if (result == SOCKET_ERROR)
		{
			cout << "클라이언트와 연결이 끊김" << endl;
			return;
		}
		CS_PLAYER_PACKET* p = reinterpret_cast<CS_PLAYER_PACKET*>(buf);

		//player_m.lock();

		playerInput.push(std::make_tuple(p, hero, clientSocket));
		//player_m.unlock();
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

		//SocketUtils::SetTcpNoDelay(clientSocket, true);

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