#include "ThreadManager.h"

//using HeroRef = std::shared_ptr<class Hero>;
ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	lock_guard<mutex> guard(_lock);

	_threads.emplace_back(thread([=]()
		{
			InitTLS();
			
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static atomic<int> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
	
}

void ThreadManager::DestroyTLS()
{

}