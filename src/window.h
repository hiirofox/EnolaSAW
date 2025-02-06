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
		int width = 0;
		int height = 0;

		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->ResizeWindowCB(width, height);
			win->width = width;
			win->height = height;
		}

		static void WindowCloseCallback(GLFWwindow* window)
		{
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->CloseWindowCB();
		}

		virtual void CreateWindowCB(GLFWwindow* window)
		{

		}
		virtual void ResizeWindowCB(int width, int height)//为了MainComponent加的
		{

		}
		void CloseWindowCB()
		{
			if (window)
			{
				Close();
				glfwDestroyWindow(window);
				window = NULL;
			}
		}
	protected:
		GLFWwindow* window = NULL;
	public:
		Window()
		{
		}
		~Window()
		{
		}
		GLFWwindow* getWindow()
		{
			return window;
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
			CreateWindowCB(window);
			this->width = width;
			this->height = height;
		}

		int GetWindowWidth()
		{
			return width;
		}
		int GetWindowHeight()
		{
			return height;
		}

		void UpdataWindowEvent()//幸苦一下了，在main循环里调用这个
		{
			if (window)
			{
				glfwMakeContextCurrent(window);
				glfwPollEvents();
			}
		}

		virtual void Close()//如果窗口关闭，这个会被调用
		{
			DBG("closeWindow\n");
		}
	};
}