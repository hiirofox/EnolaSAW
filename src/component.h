#pragma once

#include "glfw\glfw3.h"
#include "type.h"
#include "dbg.h"
#include "graphics.h"
#include "mutex.h"

/*
处理逻辑：XXXCallback->XXXCallbackProc->群发到每一个子控件->子控件的XXXProc
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
			Resize(windowRect);//先给parent控件设置完，再传递给子控件
			Rectangle myrect = { parentPos.x + myBounds.x,parentPos.y + myBounds.y ,myBounds.w,myBounds.h };
			g.ResizeProc(pwh, windowRect, myrect);
			for (auto& child : childComponents)
			{
				child->ResizeCallbackProc(pwh, windowRect, { parentPos.x + myBounds.x,parentPos.y + myBounds.y });
			}
		}
		int MouseMsgCallbackProc(MouseMsg& msg)//鼠标事件是一层层往上传（传给child），如果无人认领就是自己的了。
		{
			int flag = 0;
			for (auto& child : childComponents)
			{
				if (child->MouseMsgCallbackProc(msg))//有人要了
				{
					flag = 1;
				}
			}
			if (flag == 1)return 1;//有人要了就润
			int rx = parentPos.x + myBounds.x;
			int ry = parentPos.y + myBounds.y;
			if (
				msg.x >= rx &&
				msg.y >= ry &&
				msg.x <= rx + myBounds.w &&
				msg.y <= ry + myBounds.h)//没人要就看看是不是自己的，是就要
			{
				//todo 处理鼠标事件回调
				//当然，也可以做一些选项，即无视上层控件阻挡
				MouseMsgProc(msg);
				return 1;
			}
			return 0;//不是自己的，也没人要
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