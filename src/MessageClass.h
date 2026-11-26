#pragma once
#include <iostream>

#define ARROWLEFT 0
#define ARROWRIGHT 1

class Messege 
{
private:
    std::string messege;
    int indexOfSymbol;

public:
    Messege();

    std::string getMessege() const;

    void deleteMessege();
    void backspaceMessege();
    void addSybmol(char symbol);
    bool arrowsFunc(int arrow);
    bool isEmpty();
    const char* c_str();
    int size();
    void messegeOut(const char* messegeOut, int length);
    int getPositionOfCursor();
};