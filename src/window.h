#pragma once

#include <Windows.h>
#include "glfw\glfw3.h"
#include "timer.h"
#include "dbg.h"

namespace Enola
{
	class Window  //���ֻ��һ�����࣬���Ƽ�ֱ��ʹ��
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
		virtual void ResizeWindowCB(int width, int height)//Ϊ��MainComponent�ӵ�
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
		void Create(int width, int height, const char* title)//һ��Windowֻ�ܴ���һ������
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

		void UpdataWindowEvent()//�ҿ�һ���ˣ���mainѭ����������
		{
			if (window)
			{
				glfwMakeContextCurrent(window);
				glfwPollEvents();
			}
		}

		virtual void Close()//������ڹرգ�����ᱻ����
		{
			DBG("closeWindow\n");
		}
	};
}