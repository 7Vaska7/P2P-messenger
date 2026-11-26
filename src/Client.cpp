#include <iostream>
#include <string>
#include <limits>
#include <thread>

#include "NetworkUtilsP.h"
#include "ClientUtils.h"
#include "MessegesOutMutexClass.h"


int startClient(MessegesMutex& mmOut, MessegesMutex& mmIn)
{
    std::cout << "Connected to server!\n";

    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::cout << ">> ";

    ClientUtils clientUtils;
    thread tMessege(&ClientUtils::messegesGetter, ref(clientUtils), ref(mmOut), ref(mmIn));
    
    while (true)
    {
        string temp = mmIn.pop();
        std::cout << temp;

        if (temp[0] == '\x1A')
        {
            break;
        }
    }
}
