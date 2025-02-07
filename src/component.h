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
		Rectangle myBounds;
		//todo bounds��xy����Ե�
	protected:
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
			if (
				msg.x >= myBounds.x &&
				msg.y >= myBounds.y &&
				msg.x <= myBounds.x + myBounds.w &&
				msg.y < myBounds.y + myBounds.h)//û��Ҫ�Ϳ����ǲ����Լ��ģ��Ǿ�Ҫ
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
		virtual void Resize(int w, int h)
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