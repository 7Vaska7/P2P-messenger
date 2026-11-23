#include "MessageClass.h"

Messege::Messege() : indexOfSymbol(0) {}

std::string Messege::getMessege() const 
{
    return messege;
}

void Messege::deleteMessege() 
{
    messege.clear();
    indexOfSymbol = 0;
}

void Messege::backspaceMessege() 
{
    if (!messege.empty() && indexOfSymbol > 0) 
    {
        messege.erase(indexOfSymbol - 1, 1);
        indexOfSymbol--;
    }
}

void Messege::addSybmol(char symbol) 
{
    messege.insert(indexOfSymbol, 1, symbol);
    indexOfSymbol++;
}

void Messege::arrowsFunc(int arrow) 
{
    if (arrow == ARROWLEFT) 
    {
        if (indexOfSymbol > 0) 
        {
            indexOfSymbol--;
        }
    }
    else if (arrow == ARROWRIGHT) 
    {
        if (indexOfSymbol < messege.length()) 
        {
            indexOfSymbol++;
        }
    }
}

bool Messege::isEmpty()
{
    return messege.empty();
}

const char* Messege::c_str()
{
    return messege.c_str();
}

int Messege::size()
{
    return messege.size();
}