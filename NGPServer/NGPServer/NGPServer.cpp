#include "pch.h"
#include "protocol.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include <mutex>


std::mutex g_m;
queue<CS_PLAYER_PACKET*> playerInput;
mutex player_m;

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
void CalculateThread(SOCKET clientSocket)
{
	while (true)
	{
		//todo

		player_m.lock();
		while (!playerInput.empty())
		{
			CS_PLAYER_PACKET* playerCalculate = playerInput.front();
			playerInput.pop();


			playerCalculate->player_hp = 100;
			SC_PLAYER_PACKET p;
			p.player_hp = playerCalculate->player_hp;
			int size = sizeof(p);
			send(clientSocket, reinterpret_cast<char*>(&size), sizeof(size), 0);
			int result = send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);
			if (result == SOCKET_ERROR) {
				std::cout << "Failed to send data" << std::endl;
				
			}
			
			player_m.lock();

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

		//player패킷 테스트!!
	/*	cout << "player_id : " << p->player_id << endl;
		cout << "player_status : " << p->status << endl;
		cout << "player_ready : " << p->ready  << endl;
		cout << "Player_key.is_w : " << p->Player_key.is_w << endl;
		cout << "Player_key.is_a : " << p->Player_key.is_a << endl;
		cout << "Player_key.is_s : " << p->Player_key.is_s << endl;
		cout << "Player_key.is_d : " << p->Player_key.is_d << endl;
		cout << "Player_key.is_q : " << p->Player_key.is_q << endl;

		cout << "Player_key.is_space : " << p->Player_key.is_space << endl;
		cout << "Player_key.is_bullet : " << p->Player_key.is_bullet << endl;
		cout << "Player_key.dirx : " << p->Player_key.dirx << endl;
		cout << "Player_key.diry : " << p->Player_key.diry << endl;
		cout << "Player_key.dirz : " << p->Player_key.dirz << endl;*/


		player_m.lock();

		playerInput.push(p);
	
		player_m.unlock();

		

		
	

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
		thread calculationThread(CalculateThread, clientSocket);
		calculationThread.join();

	}
	threadManager.Join();
	


	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}