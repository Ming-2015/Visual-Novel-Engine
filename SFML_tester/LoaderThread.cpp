#include "LoaderThread.h"

std::mutex LoaderThread::diskMutex;
std::mutex LoaderThread::nextToAccessMutex;
std::mutex LoaderThread::lowPriorityAccessMutex;

void LoaderThread::launch()
{
	std::string msg;

	startedLoading = true;

	while (true)
	{
		// if low priority, try to lock the access mutex
		if (lowPriority)
		{
			// failed to lock, keep trying
			if (!lowPriorityAccessMutex.try_lock())
			{
				std::this_thread::yield();
			}
			// locked! break away from loop
			else
			{
				//msg = getPath() + ": Locked Low Priority access mutex";
				//LOGGER->Log("LoaderThread", msg);
				lowPriorityMutexLocked = true;
				break;
			}
		}
		// if high priority, break right away
		else
		{
			//msg = getPath() + ": Skipped Low Priority access mutex";
			//LOGGER->Log("LoaderThread", msg);
			break;
		}
	}

	// if the low priority is locked OR thread is set to high priority
	while (!nextToAccessMutex.try_lock())
	{
		std::this_thread::yield();
	}

	//msg = getPath() + ": Locked next-to-access mutex";
	//LOGGER->Log("LoaderThread", msg);

	// obtained the next to access mutex lock!
	// now, try to lock the disk access
	while (!diskMutex.try_lock())
	{
		std::this_thread::yield();
	}

	//msg = getPath() + ": Locked disk mutex";
	//LOGGER->Log("LoaderThread", msg);

	// unlock the next to access mutex, so that other threads can lock it
	nextToAccessMutex.unlock();
	//msg = getPath() + ": Unlocked next-to-access mutex";
	//LOGGER->Log("LoaderThread", msg);

	// assuming startloading will involve a disk-intensive operation
	// this means one thread should have full access to the disk reader one at a time
	// thus, we are locking the mutex 
	startLoading();
	msg = getPath() + ": Started loading!";
	LOGGER->Log("LoaderThread", msg);

	// unlock the disk access
	diskMutex.unlock();
	//msg = getPath() + ": unlocked disk mutex";
	//LOGGER->Log("LoaderThread", msg);

	// unlock the low priority access mutex
	if (lowPriorityMutexLocked)
	{
		lowPriorityMutexLocked = false;
		lowPriorityAccessMutex.unlock();
		//msg = getPath() + ": unlocked low priority access mutex";
		//LOGGER->Log("LoaderThread", msg);
	}

	doneLoading = true;
}
