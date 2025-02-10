#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include "waveout.h"
#include "window.h"
#include "type.h"
#include "component.h"
#include "window.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Enola {
	class TestComponent :public Component
	{
	private:
	public:
		void Paint(Graphics& g) override
		{
			//g.Clear(0x00ff00ff);
			g.SetColor(0xff0000ff);
			g.DrawRectangle(0, 0, 128, 128);
		}
		void Resize(Rectangle windowRect) override
		{

		}
		void MouseMsgProc(MouseMsg& m) override
		{
			DBG("test1 :%d %d\n", m.x, m.y);
		}
	};
	class App1 :public Window
	{
	private:
		TestComponent test1;
	public:
		App1()
		{
			AddAndMakeVisable(test1);
		}
		void Paint(Graphics& g)
		{
			//g.Clear(0x000000ff);
			g.SetColor(0x00ff00ff);
			g.DrawRectangle(64, 64, 512 + 64, 256 + 64);
		}
		void Resize(Rectangle windowRect) override
		{
			test1.SetBounds({ 64,64,512,256 });
		}
		void MouseMsgProc(MouseMsg& m) override
		{
			DBG("window:%d %d\n", m.x, m.y);
		}
	};
}
Enola::App1 app1;
int main()
{
	app1.Create((std::string)"hello l-model!", 640, 480);
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}