//Copyright bHaptics Inc. 2017
#ifndef BHAPTICS_TIMER
#define BHAPTICS_TIMER

#include <chrono>
#include <thread>
#include <deque>
#include <mutex>
#include <future>

namespace bhaptics
{
    using namespace std;
    class HapticTimer
    {
    public:
        ~HapticTimer()
        {
            stop();
        }

        void start()
        {
            if (!started)
            {
                started = true;
                runner = std::thread(&HapticTimer::workerFunc, this);
            }
        }

        void addTimerHandler(std::function<void()> &callback)
        {
            callbackFunc = callback;
        }

        void stop()
        {
            started = false;
            if (runner.joinable())
            {
                runner.join();
            }
        }
    private:
        std::atomic<bool> started = false;
        std::function<void()> callbackFunc;
        int interval = 20;
        int sleepTime = 3;

        std::chrono::steady_clock::time_point prev;

        void workerFunc()
        {
            while (started)
            {
                std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
                
                int values = std::chrono::duration_cast<std::chrono::milliseconds>(current - prev).count();
    
                if (callbackFunc && values >= interval )
                {
                    prev = current;
                    callbackFunc();
                }

                std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
            }
        }
        std::thread runner;
    };
}

#endif