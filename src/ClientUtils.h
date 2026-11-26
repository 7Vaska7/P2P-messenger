#pragma once

#include "MessageClass.h"
#include "MessegesOutMutexClass.h"

class ClientUtils
{
private:
	bool canWrite = true;

public:
	Messege messege;

	void messegesGetter(MessegesMutex& mmOut, MessegesMutex& mmIn);
};

