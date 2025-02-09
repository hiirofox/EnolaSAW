#pragma once

#include "glfw\glfw3.h"
#include "dbg.h"

/*
�����߼���XXXCallback->XXXCallbackProc->Ⱥ����ÿһ���ӿؼ�->�ӿؼ���XXXProc
*/

namespace Enola
{
	typedef struct
	{
		int x, y;
	}Positive;
	typedef struct
	{
		int x, y, w, h;
	}Rectangle;
	typedef struct
	{
		int button, action, mods;
		double x, y;
	}MouseMsg;

	class Component
	{
	private:
		std::vector<std::unique_ptr<Component>> childComponents;
		Positive parentPos;
		Rectangle myBounds;
	protected:
		void ResizeCallbackProc(Rectangle& windowRect, Positive nowPos)
		{
			parentPos = nowPos;
			Resize(windowRect);//�ȸ�parent�ؼ������꣬�ٴ��ݸ��ӿؼ�
			for (auto& child : childComponents)
			{
				child->ResizeCallbackProc(windowRect, { nowPos.x + myBounds.x,nowPos.y + myBounds.y });
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
			int ry = parentPos.x + myBounds.x;
			if (
				msg.x >= rx &&
				msg.y >= ry &&
				msg.x <= rx + myBounds.w &&
				msg.y < ry + myBounds.h)//û��Ҫ�Ϳ����ǲ����Լ��ģ��Ǿ�Ҫ
			{
				//todo ��������¼��ص�
				//��Ȼ��Ҳ������һЩѡ��������ϲ�ؼ��赲
				MouseMsgProc(msg);
				return 1;
			}
			return 0;//�����Լ��ģ�Ҳû��Ҫ
		}
		void AddAndMakeVisable(Component& p)
		{
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
		virtual void Paint(GLFWwindow* pwh)
		{
		}
		virtual void MouseMsgProc(MouseMsg& msg)
		{
			//DBG("x:%.0f y:%.0f button:%d action:%d mods:%d\n", msg.x, msg.y, msg.button, msg.action, msg.mods);
		}
	};
}