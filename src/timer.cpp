#include "timer.h"

using namespace Enola;

Timer::Timer() : timerThread() {}

Timer::~Timer()
{
	flag = 0;//我也不知道线程退没退出
	if (timerThread.joinable())
	{
		timerThread.join();
	}
	//将就用了
}

void Timer::Start(int freqHz)
{
	flag = 1;
	timerThread = std::thread([this, freqHz]()
		{
			while (flag)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1000000 / freqHz));//这个精度不高
				if (timerCallback) {
					timerCallback();
				}
			}
		});
}

void Timer::RegTimerCallback(std::function<void(void)> cbfunc) {
	timerCallback = cbfunc;
}
