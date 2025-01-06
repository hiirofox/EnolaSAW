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

		}
		void Close()
		{
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		void UpdataWindowEvent()//�ҿ�һ���ˣ���mainѭ����������
		{
			glfwMakeContextCurrent(window);
			glfwPollEvents();
		}

		virtual void resize(int width, int height)//������ڴ�С�ı䣬����ᱻ����
		{
			DBG("resize:%d %d\n", width, height);
		}

		virtual void closeWindow()//������ڹرգ�����ᱻ����
		{
			DBG("closeWindow\n");
			Close();
		}
	};
}