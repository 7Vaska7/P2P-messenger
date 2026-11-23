#include <iostream>
#include <limits>
#include <thread>

#include "ClientP.h"
#include "ServerP.h"

using namespace std;

int main()
{
    thread servert(startServer);
    thread clientt(startClient);

    servert.join();
    clientt.join();

    //std::cout << "\nPress Enter to exit...";
    //std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    //std::cin.get();
}