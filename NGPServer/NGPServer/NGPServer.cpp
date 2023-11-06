#include "pch.h"
#include "protocol.h"
#include "ThreadManager.h"
#include "SocketUtils.h"

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
	SC_PLAYER_PACKET p;

	// 클라이언트에게 스레드 ID를 보내기 위한 작업
	p.player_id = LThreadId;
	send(clientSocket, reinterpret_cast<char*>(&p), sizeof(p), 0);

	// 나머지 클라이언트 소켓 처리 코드
	while (true)
	{
		char recvBuffer[1000];
		int recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen == SOCKET_ERROR)
		{
			cout << "클라이언트와 연결이 끊김" << endl;
			break;
		}

		cout << "Recv Data = " << recvBuffer << endl;
		cout << "Recv Data len = " << recvLen << endl;
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