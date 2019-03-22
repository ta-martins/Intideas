#include "ThreadManager.h"

ThreadManager::~ThreadManager()
{
	for (auto o : obj)
	{
		if (o.second->joinable())
		{
			flags[o.first]->store(THREAD_HALT);
			o.second->join();
			delete o.second;
			std::cerr << "Thread 0x" << std::hex << o.first << " is joining this thread [0x" << std::hex << std::this_thread::get_id() << "]. Waiting" << std::endl;
		}
		else
		{
			flags[o.first]->store(THREAD_HALT);
			o.second->detach();
			std::cerr << "Thread 0x" << std::hex << o.first << " is not joinable. Halting and detaching" << std::endl;
		}
		delete flags[o.first];
	}

	flags.clear();
	obj.clear();
}

std::thread::id ThreadManager::addThread(std::function<void(std::atomic<int>*, void*)> threadFunction, void* threadData)
{
	std::thread * thread;
	/* Init a thread with the running status obviously */
	std::atomic<int> * flag = new std::atomic<int>(THREAD_RUN);
	if (threadData != nullptr)
		thread = new std::thread(threadFunction, flag, threadData);
	else
		thread = new std::thread(threadFunction, flag, nullptr);

	std::thread::id id = thread->get_id();

	obj.emplace(id, thread);
	flags.emplace(id, flag);

	return id;
}

bool ThreadManager::joinThreadSync(std::thread::id threadId)
{
	if (obj[threadId]->joinable())
	{
		flags[threadId]->store(THREAD_HALT);
		obj[threadId]->join();

		std::cerr << "Thread 0x" << std::hex << threadId << " joined successfully" << std::endl;

		delete flags[threadId];
		delete obj[threadId];

		obj.erase(threadId);
		flags.erase(threadId);

		return true;
	}

	std::cerr << "Thread 0x" << std::hex << threadId << " is not joinable. Ignoring" << std::endl;
	return false;
}

bool ThreadManager::joinThreadAsync(std::thread::id threadId)
{
	flags[threadId]->store(THREAD_HALT);
	obj[threadId]->detach();

	std::cerr << "Thread 0x" << std::hex << threadId << " is being detached" << std::endl;

	delete flags[threadId];

	obj.erase(threadId);
	flags.erase(threadId);

	return true;
}

bool ThreadManager::signalThreadSetStatus(std::thread::id threadId, int status)
{
	if (status == THREAD_HALT)
	{
		std::cerr << "Thread 0x" << std::hex << threadId << " cannot be halted from this method. Refer to joinThread{Sync,Async} methods" << std::endl;
		return false;
	}
	flags[threadId]->store(status);
	return true;
}

int ThreadManager::signalThreadGetStatus(std::thread::id threadId)
{
	int status = flags[threadId]->load();
	return status;
}
