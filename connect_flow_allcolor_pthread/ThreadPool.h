#ifndef __THREADPOOL_H__   /*ThreadPool.h*/
#define __THREADPOOL_H__
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

struct Task {
    void (*func)(void *);
    void *arg;
};



class ThreadPool {
public:
    ThreadPool(int size);
    ~ThreadPool();
    void addTask(void (*func)(void *), void *arg);
private:
    void _worker(int i);
    bool _stopped;
    int _size;
    std::condition_variable _cond;
    std::mutex _mutex;
    std::deque<Task> _tasks;
    std::vector<std::thread> _workers;
};
#endif