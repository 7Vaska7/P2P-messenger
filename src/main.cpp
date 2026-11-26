#include <iostream>
#include <limits>
#include <thread>
#include <future>

#include "ClientP.h"
#include "ServerP.h"
#include "MessegesOutMutexClass.h"

using namespace std;

int main()
{
    MessegesMutex mmOut;
    MessegesMutex mmIn;

    promise<void> clientStartPromise;
    future<void> clientStartFuture = clientStartPromise.get_future();

    thread servert(startServer, ref(mmOut), ref(mmIn), move(clientStartPromise));
    clientStartFuture.wait();
    thread clientt(startClient, ref(mmOut), ref(mmIn));

    servert.join();
    clientt.join();

    //std::cout << "\nPress Enter to exit...";
    //std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    //std::cin.get();
}