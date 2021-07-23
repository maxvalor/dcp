#pragma once

#include "Rate.h"
#include <thread>
#include <atomic>
#include <functional>

namespace util {

class LoopThread
{
    double hz;
    std::atomic_bool running;
    std::thread t;
    int policy = POLICY_NONE;
    int priority;
    Rate *rate;
    std::function<void(void)> init;
    std::function<void(void)> run;
public:
    enum {
        POLICY_DEFAULT  = SCHED_OTHER,
        POLICY_FIFO     = SCHED_FIFO,
        POLICY_RR       = SCHED_RR,
        POLICY_NONE = SCHED_OTHER + SCHED_FIFO + SCHED_RR
    };

    LoopThread(double hz, std::function<void(void)> run)
        : hz(hz), init(nullptr), run(run)
    {
        running = false;
        rate = new Rate(hz);
        priority = 0;
    }

    LoopThread(double hz, std::function<void(void)> init, std::function<void(void)> run)
        : hz(hz), init(init), run(run)
    {
        running = false;
        rate = new Rate(hz);
        priority = 0;
    }

    LoopThread(double hz,
        int policy, int priority, std::function<void(void)> run)
        : hz(hz), policy(policy), priority(priority), init(nullptr), run(run)
    {
        running = false;
        rate = new Rate(hz);
    }

    LoopThread(double hz, 
        int policy, int priority, std::function<void(void)> init, std::function<void(void)> run)
        : hz(hz), policy(policy), priority(priority), init(init), run(run)
    {
        running = false;
        rate = new Rate(hz);
    }

    ~LoopThread()
    {
        if (running)
        {
            Stop();
        }

        if (rate != nullptr)
            delete rate;
    }
    

    double GetHz() const
    {
        return hz;
    }

    void Start()
    {
        running = true;
        auto packed_f = [this]()
        {
            if (init != nullptr)
            {
                init();
            }
            while (running)
            {
                run();
                rate->sleep();
            }
        };
        t = std::thread(packed_f);
        if (policy != POLICY_NONE)
        {
            sched_param param = {priority};
            pthread_setschedparam(t.native_handle(), policy, &param);
        }
    }

    bool Stop(bool blocked = true)
    {
        running = false;
        rate->interrupt();

        try
        {
            blocked ? t.join() : t.detach();
        }
        catch(const std::exception& e)
        {
            return false;
        }

        return true;
    }
};

}
