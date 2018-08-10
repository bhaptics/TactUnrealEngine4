//Copyright bHaptics Inc. 2017
#include "timer.h"
HapticTimer::~HapticTimer()
{
    stop();
}

void HapticTimer::start()
{
    if (!started)
    {
        started = true;
        runner = std::thread(&HapticTimer::workerFunc, this);
    }
}

void HapticTimer::addTimerHandler(std::function<void()> &callback)
{
    callbackFunc = callback;
}

void HapticTimer::stop()
{
    started = false;
    if (runner.joinable())
    {
        runner.join();
    }
}

void HapticTimer::workerFunc()
{
    while (started)
    {
        std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
                
        int values = std::chrono::duration_cast<std::chrono::milliseconds>(current - prev).count();
		bool isIntervalOver = (current > (prev + std::chrono::milliseconds(interval)));

		if(callbackFunc && isIntervalOver)
        {
            prev = current;
            callbackFunc();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}
