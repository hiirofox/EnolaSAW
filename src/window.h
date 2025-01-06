#pragma once

#include <Windows.h>
#include "glfw\glfw3.h"
#include "timer.h"
#include "dbg.h"

namespace Enola
{
	class Window  //这个只是一个基类，不推荐直接使用
	{
	private:
		GLFWwindow* window = NULL;

		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->resize(width, height);
		}

		static void WindowCloseCallback(GLFWwindow* window)
		{
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->closeWindow();
		}

	public:
		Window()
		{
		}
		~Window()
		{
			glfwDestroyWindow(window);
		}

		void Create(int width, int height, const char* title)//一个Window只能创建一个窗口
		{
			window = glfwCreateWindow(width, height, title, NULL, NULL);
			if (!window)
			{
				glfwTerminate();
				DBG("glfwCreateWindow failed\n");
				while (1);
			}
			glfwSetWindowUserPointer(window, this);
			glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
			glfwSetWindowCloseCallback(window, WindowCloseCallback);

		}
		void Close()
		{
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		void UpdataWindowEvent()//幸苦一下了，在main循环里调用这个
		{
			glfwMakeContextCurrent(window);
			glfwPollEvents();
		}

		virtual void resize(int width, int height)//如果窗口大小改变，这个会被调用
		{
			DBG("resize:%d %d\n", width, height);
		}

		virtual void closeWindow()//如果窗口关闭，这个会被调用
		{
			DBG("closeWindow\n");
			Close();
		}
	};
}