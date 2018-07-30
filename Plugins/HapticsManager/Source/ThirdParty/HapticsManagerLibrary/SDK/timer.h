//Copyright bHaptics Inc. 2017
#ifndef BHAPTICS_TIMER
#define BHAPTICS_TIMER

#include <chrono>
#include <thread>
#include <deque>
#include <mutex>
#include <future>
#include <atomic>

    class HapticTimer
    {
    public:
		~HapticTimer();

		void start();

		void addTimerHandler(std::function<void()> &callback);

		void stop();

    private:
        std::atomic<bool> started = false;
        std::function<void()> callbackFunc;
        int interval = 100;
        int sleepTime = 5;

        std::chrono::steady_clock::time_point prev;

		void workerFunc();
        std::thread runner;
    };


#endif