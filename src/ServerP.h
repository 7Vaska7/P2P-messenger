#pragma once

#include "MessegesOutMutexClass.h"

int startServer(MessegesMutex& mmOut, MessegesMutex& mmIn, std::promise<void> clientPromise);