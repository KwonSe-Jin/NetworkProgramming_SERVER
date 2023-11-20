#pragma once

#include "pch.h"
//#include "TLS.h"

/*------------------
	ThreadManager
-------------------*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void	Launch(function<void(void)> callback);
	void	Join();

private:
	static void InitTLS();
	static void DestroyTLS();

private:
	mutex			_lock;
	vector<thread>	_threads;
};

