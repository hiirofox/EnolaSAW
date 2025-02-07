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

		static void ResizeCallback(GLFWwindow* pWnd, int w, int h)
		{
			Window* my = (Window*)glfwGetWindowUserPointer(pWnd);
			my->width = w;
			my->height = h;
			my->Resize(w, h);
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
			glfwSetWindowUserPointer(pWndHandle, this);
			glfwSetWindowSizeCallback(pWndHandle, ResizeCallback);
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
			Resize(width, height);
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