#pragma once

#include "glfw\glfw3.h"
#include "type.h"
#include "dbg.h"
#include "graphics.h"
#include "mutex.h"

/*
�����߼���XXXCallback->XXXCallbackProc->Ⱥ����ÿһ���ӿؼ�->�ӿؼ���XXXProc
*/

extern std::mutex WndProcMtx;
namespace Enola
{
	class Component
	{
	private:
		std::vector<std::unique_ptr<Component>> childComponents;
		Positive parentPos = { 0,0 };
		Rectangle myBounds = { 0,0 };
		GLFWwindow* pwh = NULL;
		Graphics g;
	protected:
		void RepaintCallbackProc(GLFWwindow* pwh)
		{
			g.UpdataRectangle();
			Paint(g);
			for (auto& child : childComponents)
			{
				child->SetGLFWwindow(pwh);
				child->RepaintCallbackProc(pwh);
			}
		}
		void ResizeCallbackProc(GLFWwindow* pwh, Rectangle& windowRect, Positive parentPos)
		{
			this->parentPos = parentPos;
			this->pwh = pwh;
			Resize(windowRect);//�ȸ�parent�ؼ������꣬�ٴ��ݸ��ӿؼ�
			Rectangle myrect = { parentPos.x + myBounds.x,parentPos.y + myBounds.y ,myBounds.w,myBounds.h };
			g.ResizeProc(pwh, windowRect, myrect);
			for (auto& child : childComponents)
			{
				child->ResizeCallbackProc(pwh, windowRect, { parentPos.x + myBounds.x,parentPos.y + myBounds.y });
			}
		}
		int MouseMsgCallbackProc(MouseMsg& msg)//����¼���һ������ϴ�������child�������������������Լ����ˡ�
		{
			int flag = 0;
			for (auto& child : childComponents)
			{
				if (child->MouseMsgCallbackProc(msg))//����Ҫ��
				{
					flag = 1;
				}
			}
			if (flag == 1)return 1;//����Ҫ�˾���
			int rx = parentPos.x + myBounds.x;
			int ry = parentPos.y + myBounds.y;
			if (
				msg.x >= rx &&
				msg.y >= ry &&
				msg.x <= rx + myBounds.w &&
				msg.y <= ry + myBounds.h)//û��Ҫ�Ϳ����ǲ����Լ��ģ��Ǿ�Ҫ
			{
				//todo ��������¼��ص�
				//��Ȼ��Ҳ������һЩѡ��������ϲ�ؼ��赲
				MouseMsgProc(msg);
				return 1;
			}
			return 0;//�����Լ��ģ�Ҳû��Ҫ
		}
		void SetGLFWwindow(GLFWwindow* pwh)
		{
			this->pwh = pwh;
		}
		void AddAndMakeVisable(Component& p)
		{
			p.SetGLFWwindow(pwh);
			childComponents.emplace_back(&p);
		}
	public:
		void SetBounds(Rectangle bounds)
		{
			myBounds = bounds;
		}
		virtual void Resize(Rectangle windowRect)
		{
		}
		virtual void Paint(Graphics& g)
		{
		}
		void Repaint()
		{
			WndProcMtx.lock();
			glfwMakeContextCurrent(pwh);
			RepaintCallbackProc(pwh);
			glfwSwapBuffers(pwh);
			WndProcMtx.unlock();
		}
		virtual void MouseMsgProc(MouseMsg& msg)
		{
			//DBG("x:%.0f y:%.0f button:%d action:%d mods:%d\n", msg.x, msg.y, msg.button, msg.action, msg.mods);
		}
	};
}