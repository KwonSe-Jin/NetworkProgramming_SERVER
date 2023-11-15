#include "pch.h"
#include "protocol.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include <mutex>


std::mutex g_m;

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

// 게임 논리를 처리할 계산 스레드
void CalculateThread()
{
	while (true)
	{
		//todo
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


	while (true)
	{
		//클라이언트에서 키입력 받기 고정길이 // 가변길이 방식
		/*char buf[100];
		int size;
		recv(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
		std::cout << size << "바이트 받음" << std::endl;
		int result = recv(clientSocket, buf, size, MSG_WAITALL);
		if (result == SOCKET_ERROR)
		{
			cout << "클라이언트와 연결이 끊김" << endl;
			return;
		}
		CS_PLAYER_PACKET* p = reinterpret_cast<CS_PLAYER_PACKET*>(buf);*/

		//player패킷 테스트!!
		//cout << "player_id : " << p->player_id << endl;
		//cout << "player_status : " << p->status << endl;
		//cout << "player_ready : " << p->ready  << endl;
		//cout << "Player_key.is_w : " << p->Player_key.is_w << endl;
		//cout << "Player_key.is_a : " << p->Player_key.is_a << endl;
		//cout << "Player_key.is_s : " << p->Player_key.is_s << endl;
		//cout << "Player_key.is_d : " << p->Player_key.is_d << endl;
		//cout << "Player_key.is_space : " << p->Player_key.is_space << endl;
		//cout << "Player_key.is_bullet : " << p->Player_key.is_bullet << endl;
		//cout << "Player_key.dirx : " << p->Player_key.dirx << endl;
		//cout << "Player_key.diry : " << p->Player_key.diry << endl;
		//cout << "Player_key.dirz : " << p->Player_key.dirz << endl;

		int type = rand() % 3 + 1;
		switch (type) {
		case 1: {
			SC_PLAYER_PACKET p;
			p.packet_type = SC_PLAYER;
			p.Player_pos.x = rand() % 19;
			p.Player_pos.y = rand() % 19;
			p.Player_pos.z = rand() % 19;


			std::cout << "송신 - ";
			int size = sizeof(p);
			send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
			int retval = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);

			cout << "패킷 타입" << p.packet_type << endl;
			std::cout << "좌표 데이터 x, y, z : " << p.Player_pos.x << ", " << p.Player_pos.y << ", " << p.Player_pos.z << std::endl;

		}
			  break;
		case 2: {
			SC_MONSTER_PACKET p;
			p.packet_type = SC_MONSTER;
			p.animal_type = CAT;

			std::cout << "송신 - ";
			int size = sizeof(p);
			send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
			int retval = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);

			cout << "패킷 타입" << p.packet_type << endl;
			std::cout << "애니멀 타입 : " << p.animal_type << std::endl;
		}
			  break;
		case 3: {
			SC_BULLET_PACKET p;
			p.packet_type = SC_BULLET;
			p.dirx = rand() % 19;
			p.diry = rand() % 19;
			p.dirz = rand() % 19;

			std::cout << "송신 - ";
			int size = sizeof(p);
			send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
			int retval = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);

			cout << "패킷 타입" << p.packet_type << endl;
			std::cout << "좌표 데이터 x, y, z : " << p.dirx << ", " << p.diry << ", " << p.dirz << std::endl;
		}
			  break;
		}
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds{ 1 });

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
			return 0 ;
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