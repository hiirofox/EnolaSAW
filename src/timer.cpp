#include "timer.h"

using namespace Enola;

Timer::Timer() : timerThread() {}

Timer::~Timer()
{
	flag = 0;//��Ҳ��֪���߳���û�˳�
	if (timerThread.joinable())
	{
		timerThread.join();
	}
	//��������
}

void Timer::Start(int freqHz)
{
	flag = 1;
	timerThread = std::thread([this, freqHz]()
		{
			while (flag)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1000000 / freqHz));//������Ȳ���
				if (timerCallback) {
					timerCallback();
				}
			}
		});
}

void Timer::RegTimerCallback(std::function<void(void)> cbfunc) {
	timerCallback = cbfunc;
}
