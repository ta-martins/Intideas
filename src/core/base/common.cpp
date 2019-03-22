#include "common.h"

bool handleError(int32 error, const char * funcName)
{
	if (error < 0)
	{
		char buffer[2048];
		DAQmxGetExtendedErrorInfo(buffer, 2048);
		std::cerr << funcName << " returned error: " << buffer << std::endl;
		return true;
	}
	else if (error > 0)
	{
		char buffer[2048];
		DAQmxGetExtendedErrorInfo(buffer, 2048);
		std::cerr << funcName << " returned warning: " << buffer << std::endl;
		return true;
	}
	return false;
}
