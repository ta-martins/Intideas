#include "worker_threads.h"
#include "worker_callbacks.h"
#include "base/Timestamp.h"
#include <chrono>

void acquireThread(std::atomic<int>* flags, void * data)
{
	std::cout << "Acquiring thread [0x" << std::hex << std::this_thread::get_id() << "] started." << std::endl;

	AcquireDataOptions * ado = reinterpret_cast<AcquireDataOptions*>(data);

	Task task(ado->tproperties.name);

	task.addChannel(Task::VoltageChannel, ado->tproperties.channel);
	task.addTimer(ado->tproperties.timer);
	task.addEventCallback(EveryNCallback, nullptr, ado->tproperties.timer.samplesPerChannel, 0);

	Timestamp time;
	task.start(); //This marks the starting time of acquisition (NI-DAQmx reference manual)
	time.setTimeZeroPointNow();

	while (flags->load() == THREAD_RUN)
	{
		std::this_thread::yield();
	}

	task.stop();
}

void processThread(std::atomic<int>* flags, void * data)
{
	std::cout << "Processing thread [0x" << std::hex << std::this_thread::get_id() << "] started." << std::endl;

	while (true)
	{
		while (CallbackPacket::getGlobalCBPStack()->empty() && flags->load() == THREAD_RUN)
		{
			//Use sleep_for() or yield() ? 
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}

		if (flags->load() == THREAD_HALT && CallbackPacket::getGlobalCBPStack()->empty())
			break;


		//Get ready to process data
		CallbackPacket::getGlobalCBPMutex()->lock();
		auto cbp_stack = CallbackPacket::getGlobalCBPStack();
		auto dpacket = cbp_stack->front();
		cbp_stack->pop_front();
		CallbackPacket::getGlobalCBPMutex()->unlock();

		//Process the data
		static int dpser = 0;
		FILE* f = reinterpret_cast<FILE*>(data);
		for (int i = 0; i < dpacket.data_size; i++)
			fprintf(f, "%d, %d, %lf,\n", dpser, i, dpacket.data[i]);
		dpser++;

		//Free data copy
		free(dpacket.data);
	}
}