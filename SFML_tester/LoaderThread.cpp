#include "LoaderThread.h"

std::mutex LoaderThread::diskMutex;

void LoaderThread::launch()
{
	startedLoading = true;

	diskMutex.lock();

	// assuming startloading will involve a disk-intensive operation
	// this means one thread should have full access to the disk reader one at a time
	// thus, we are locking the mutex 
	std::string msg = "Started loading: " + getPath();
	LOGGER->Log("LoaderThread", msg);

	startLoading();

	msg = "Finished loading: " + getPath();
	LOGGER->Log("LoaderThread", msg);

	diskMutex.unlock();

	doneLoading = true;
}
