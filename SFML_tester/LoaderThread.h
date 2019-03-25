#pragma once

#include <string>
#include <mutex>

#include "Logger.h"

class LoaderThread
{
public:

	void launch();

	// get the path to the asset on disk
	virtual std::string getPath() = 0;

	// get the pointer to the asset object
	virtual void* getPtr() = 0;

	// return if the thread has done loading
	bool isDone() const { return doneLoading; }

	// return if the loaded asset is valid
	bool isValid() const { return valid; }

	// return if the thread has started loading
	bool hasStartedLoading() const { return startedLoading; }

	// raise the priority of this thread
	void prioritize() 
	{ 
		lowPriority = false; 
	}

protected:
	bool startedLoading = false;
	bool doneLoading = false;
	bool valid = false;

	bool lowPriority = true;
	bool lowPriorityMutexLocked = false;

	static std::mutex diskMutex;
	static std::mutex nextToAccessMutex;
	static std::mutex lowPriorityAccessMutex;

	virtual void startLoading() = 0;
};
