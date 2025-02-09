#pragma once

#include <Windows.h>
#include "glfw\glfw3.h"
#include "component.h"
#include "timer.h"
#include "dbg.h"

namespace Enola
{
	static std::mutex WndProcMtx;
	class Window :public Component
	{
	private:
		std::thread wndThread;
		std::queue<std::function<void()>> tasks;
		std::string name;
		int width, height;
		GLFWwindow* pWndHandle = NULL;
		MouseMsg mousemsg;

		static void ResizeCallback(GLFWwindow* pWnd, int w, int h)
		{
			Window* my = (Window*)glfwGetWindowUserPointer(pWnd);
			my->width = w;//window类
			my->height = h;
			Rectangle windowRect = { 0,0, w, h };
			my->SetBounds(windowRect);
			my->ResizeCallbackProc(windowRect, { 0,0 });
		}
		static void MouseButtonCallback(GLFWwindow* pWnd, int button, int action, int mods)
		{
			Window* my = (Window*)glfwGetWindowUserPointer(pWnd);
			glfwMakeContextCurrent(pWnd);
			glfwGetCursorPos(pWnd, &my->mousemsg.x, &my->mousemsg.y);
			my->mousemsg.button = button;
			my->mousemsg.action = action;
			my->mousemsg.mods = mods;
			my->MouseMsgCallbackProc(my->mousemsg);
		}
		static void CursorCallback(GLFWwindow* pWnd, double x, double y)
		{
			Window* my = (Window*)glfwGetWindowUserPointer(pWnd);
			glfwMakeContextCurrent(pWnd);
			glfwGetCursorPos(pWnd, &my->mousemsg.x, &my->mousemsg.y);
			my->mousemsg.action = -1;
			my->MouseMsgCallbackProc(my->mousemsg);
		}
		void WndProc()
		{
			WndProcMtx.lock();
			pWndHandle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
			if (!pWndHandle)
			{
				WndProcMtx.unlock();
				return;
			}
			glfwMakeContextCurrent(pWndHandle);
			glfwSetWindowUserPointer(pWndHandle, this);
			glfwSetWindowSizeCallback(pWndHandle, ResizeCallback);
			glfwSetMouseButtonCallback(pWndHandle, MouseButtonCallback);
			glfwSetCursorPosCallback(pWndHandle, CursorCallback);
			WndProcMtx.unlock();

			while (!glfwWindowShouldClose(pWndHandle))
			{
				WndProcMtx.lock();
				glfwMakeContextCurrent(pWndHandle);
				while (tasks.size())//外部传入线程内的任务
				{
					auto task = tasks.front();
					tasks.pop();
					task();
				}
				Paint(pWndHandle);
				glfwSwapBuffers(pWndHandle);
				WndProcMtx.unlock();
				glfwPollEvents();//因为这个是自己线程的所以可以丢外面做
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
		}
	public:
		void Create(std::string& name, int width, int height)
		{
			glfwInit();
			this->name = name;
			this->width = width;
			this->height = height;
			wndThread = std::thread(&Window::WndProc, this);
			SetBounds({ 0,0, width, height });
			Resize({ 0,0,width,height });
		}
		void SendTaskToThread(std::function<void()> func)
		{
			tasks.push(func);
		}
	};
	class AppTest :public Window
	{
	private:
	public:
	};
}