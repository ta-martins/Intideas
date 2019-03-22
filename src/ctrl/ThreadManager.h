#pragma once
#include <iostream>
#include <thread>
#include <unordered_map>
#include <functional>
#include <atomic>

#define THREAD_HALT 0
#define THREAD_RUN 1
#define THREAD_SUSPEND 2
#define THREAD_WAIT 3

class ThreadManager
{
public:
	ThreadManager() = default;
	~ThreadManager();

	std::thread::id addThread(std::function<void(std::atomic<int>*, void*) > threadFunction, void* threadData);
	bool joinThreadSync(std::thread::id threadId);
	bool joinThreadAsync(std::thread::id threadId);

	bool signalThreadSetStatus(std::thread::id threadId, int status);
	int  signalThreadGetStatus(std::thread::id threadId);

private:
	std::unordered_map<std::thread::id, std::thread*> obj;
	std::unordered_map<std::thread::id, std::atomic<int>*> flags;
};

