#ifndef THREADPOLL_H
#define THREADPOLL_H

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPoll {
public:
    ThreadPoll(int size = 10);
    ~ThreadPoll();
    void add(std::function<void()>);

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()> > tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
};

#endif