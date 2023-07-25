#include "ThreadPool.h" /*ThreadPool.cpp*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

ThreadPool::ThreadPool(int size) {
    this->_stopped = 0;
    for (int i = 0; i < size; i++) {
        //std::thread(&ThreadPool::_worker, this);
        printf("new thread %d\n", i);
        this->_workers.push_back(std::thread(&ThreadPool::_worker, this, i));
    }
}

ThreadPool::~ThreadPool() {
    this->_stopped = 1;
    printf("threadpool stopped\n");
    this->_mutex.unlock();
    int size = this->_workers.size();
    for (int i = 0; i < size; i++) {
        this->_workers[i].join();
    }
    this->_cond.notify_all();
    printf("all thread exit\n");
}

void ThreadPool::addTask(void (*func)(void *), void *arg) {
    printf("add Task\n");
    // Task *t = new Task;
    // t->func = func;
    // t->arg = arg;
    Task t = {func, arg};
    /*printf("address0 func %p arg %p\n", t.func, t.arg);
    this->_mutex.lock();
    this->_tasks.push_back(t);
    printf("_tasks size %d\n", _tasks.size());
    this->_mutex.unlock();*/
    {
        std::unique_lock<std::mutex> lock(this->_mutex);
        if (this->_stopped)
            throw std::runtime_error("The thread pool has been stop.");
        this->_tasks.push_back(t);
    }
    this->_cond.notify_one();
    return;
}

void ThreadPool::_worker(int i) {
    std::thread::id this_id = std::this_thread::get_id();
    printf("worker are %d id %d\n", this_id, i);
    while(1) {
        /* Lock must be taken to wait on conditional variable */
        printf("get lock1\n");
        // this->_mutex.lock();
        /* Wait on condition variable, check for spurious wakeups.
           When returning from _cond.wait(), we own the lock. */
        /*std::unique_lock<std::mutex> lock(this->_mutex);
        printf("get lock2\n");
        while(this->_tasks.empty() && this->_stopped == 0) {
            printf("empty\n");
            // this->_mutex.unlock();
            this->_cond.wait(lock);
            printf("%d get signal\n", this_id);
            // this->_mutex.lock();
        }*/
        Task t;
        {
            std::unique_lock<std::mutex> lock(this->_mutex);
            this->_cond.wait(lock, [this]() { return this->_stopped || !this->_tasks.empty();});
            if (this->_stopped && this->_tasks.empty())
                return ;
            t = std::move(this->_tasks.front());
            this->_tasks.pop_front();
        }
        (*(t.func))(t.arg);
        
        /*Task t;
        if (!this->_tasks.empty()) {
            printf("%d get a task\n", i);
            // t = new Task;
            t = this->_tasks.front();
            // t->func = this->_tasks.front().func;
            // t->arg = this->_tasks.front().arg;
            printf("address1 func %p arg %p\n", t.func, t.arg);
            this->_tasks.pop_front();
            printf("_tasks size %d\n", _tasks.size());
            this->_mutex.unlock();
            printf("run function\n");
            printf("address2 func %p arg %p\n", t.func, t.arg);
            (*(t.func))(t.arg);
            printf("end function\n");
            continue;
        }
        break;
    }
    this->_mutex.unlock();*/
    }
    return ;
}
