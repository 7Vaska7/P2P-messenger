#pragma once
#include <string>
#include "MessageClass.h"
#include "MessegesOutMutexClass.h"

std::string getLocalIp();
void messegesGetter(SOCKET& clientSocket, Messege& messege, MessegesMutex* mmOut);
void messegesReader(SOCKET& clientSocket, Messege& messege, MessegesMutex* mmOut);