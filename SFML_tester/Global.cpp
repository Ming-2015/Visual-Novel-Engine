#include "Global.h"

Global * Global::global_ptr = nullptr;

Global * Global::GetGlobal()
{
	if (global_ptr == nullptr)
		global_ptr = new Global();
	return global_ptr;
}

void Global::CleanUp()
{
	if (global_ptr != nullptr)
		delete global_ptr;
}

Global::~Global()
{
}

Global::Global()
{

}