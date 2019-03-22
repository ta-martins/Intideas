#pragma once
#include "../ctrl/ThreadManager.h"
#include "Task.h"

struct AcquireDataOptions
{
	TaskProperties tproperties;
};

void acquireThread(std::atomic<int>* flags, void* data);
void processThread(std::atomic<int>* flags, void* data);
