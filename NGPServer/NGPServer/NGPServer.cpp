#include "pch.h"
#include "protocol.h"
#include "ThreadManager.h"
#include "SocketUtils.h"

#include <mutex>
#include <vector>



std::mutex g_m;
std::queue<std::pair<CS_PLAYER_PACKET*, SOCKET>> playerInput;
mutex player_m;
//전역 만든 것 
int AnimalCnt = 0;

vector<Hero> heroes; //주인공 벡터 일단 만들어놓음 나중에 맵으로 수정 후 주석 지워주세요. 


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



//// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


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

// 게임 논리를 처리할 계산 스레드
void CalculateThread()
{
	while (true)
	{
		//todo
		//std::cout << "여기 들어옴1 ???" << endl;

		player_m.lock();
		while (!playerInput.empty())
		{
			std::cout << "여기 들어옴 ???" << endl;
	/*		CS_PLAYER_PACKET* playerCalculate = playerInput.front();
			playerInput.pop();*/

			std::pair<CS_PLAYER_PACKET*, SOCKET> packetInfo = playerInput.front();
			playerInput.pop();

			CS_PLAYER_PACKET* playerCalculate = packetInfo.first;
			SOCKET clientSocket = packetInfo.second;

			playerCalculate->player_hp = 100;
			SC_PLAYER_PACKET p;
			p.player_hp = playerCalculate->player_hp;


			SendToClient(p, clientSocket);
			//std::cout << std::endl;
			//std::this_thread::sleep_for(std::chrono::seconds{ 1 });
			//player_m.lock();

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

	char recvBuffer[1000];
	int recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
	if (recvLen == SOCKET_ERROR)
	{
		cout << "클라이언트와 연결이 끊김" << endl;
		return;
	}

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

		playerInput.push(std::make_pair(p, clientSocket));
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

	Hero Hero1;
	heroes.push_back(Hero1);

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