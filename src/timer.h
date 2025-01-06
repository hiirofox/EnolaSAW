#pragma once

#include <functional>
#include <thread>
#include <chrono>
#include "dbg.h"

namespace Enola
{
	class Timer
	{
	private:
		std::function<void(void)> timerCallback;
		std::thread timerThread;
		bool flag = 1;
	public:
		Timer();
		~Timer();
		void RegTimerCallback(std::function<void(void)> cbfunc);//œ»◊¢≤·‘Ÿstart
		void Start(int freqHz);
	};
}