#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

class MessegesMutex
{
private:
    queue<string> queue_;
    mutex mutex_;
    condition_variable cv_;

public:
    void push(string packet);
    string pop();
};
