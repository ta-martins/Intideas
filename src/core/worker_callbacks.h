#pragma once
#include "Task.h"
#include <mutex>
#include <deque>

struct DataPacket
{
	float64* data;
	size_t data_size;
	int32 data_read;
};

struct CallbackPacket
{
	static std::mutex* getGlobalCBPMutex()
	{
		static std::mutex intance;
		return &intance;
	}

	static std::deque<DataPacket>* getGlobalCBPStack()
	{
		static std::deque<DataPacket> instance;
		return &instance;
	}
};

int32 __cdecl EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
