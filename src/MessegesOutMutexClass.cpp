#include "MessegesOutMutexClass.h"

void MessegesMutex::push(string packet)
{
    {
        lock_guard<mutex> lock(mutex_);
        queue_.push(move(packet));
    }
    cv_.notify_one();
}

string MessegesMutex::pop()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });

    string packet = std::move(queue_.front());
    queue_.pop();
    return packet;
}