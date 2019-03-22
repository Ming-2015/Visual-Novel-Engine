#pragma once
#include <string>

class LoaderThread
{
public:

	virtual void startLoading() = 0;
	virtual std::string getPath() = 0;
	virtual void* getPtr() = 0;

	bool isDone() const { return doneLoading; }
	bool isValid() const { return valid; }
	bool hasStartedLoading() const { return startedLoading; }

protected:
	bool startedLoading = false;
	bool doneLoading = false;
	bool valid = false;
};