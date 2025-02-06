#pragma once

#include "glfw\glfw3.h"
#include "dbg.h"

namespace Enola
{
	/*
	struct Pos
	{
		int x;
		int y;
	};

	enum class MouseButton
	{
		Left,
		Right,
		Middle
	};

	class MouseMsg
	{
	private:
		Pos pos;
	public:
		Pos GetMousePos();
		void SetMousePos(int x, int y);
		void SetMousePos(Pos pos);
		void SetVisible(bool visible);//鼠标指针可见
	};
	*/
	struct Rectangle
	{
		int x;
		int y;
		int width;
		int height;
		Rectangle operator+(Rectangle rect)
		{
			return { x + rect.x, y + rect.y, width , height };
		}
	};

	class Graphics
	{
	private:
		GLFWwindow* window = NULL;
		Rectangle root;
	protected:
		void SetDrawRectangle(Rectangle root)
		{
			this->root = root;
		}
	public:
		Graphics()
		{

		}
		void SetWindow(GLFWwindow* window)
		{
			this->window = window;
		}
		void SetBounds(Rectangle root)
		{
			this->root = root;
		}
		void SetColor(int ARGB)
		{
			glColor4b((ARGB >> 16) & 0xFF, (ARGB >> 8) & 0xFF, ARGB & 0xFF, (ARGB >> 24) & 0xFF);
		}
		void DrawLine(int x1, int y1, int x2, int y2)
		{
			glBegin(GL_LINES);
			glVertex2i(x1 + root.x, y1 + root.y);
			glVertex2i(x2 + root.y, y2 + root.y);
			glEnd();
		}
		void DrawRect(Rectangle rect)
		{
			glBegin(GL_LINE_LOOP);
			rect.x += root.x;
			rect.y += root.y;
			glVertex2i(rect.x, rect.y);
			glVertex2i(rect.x + rect.width, rect.y);
			glVertex2i(rect.x + rect.width, rect.y + rect.height);
			glVertex2i(rect.x, rect.y + rect.height);
			glEnd();
		}
		void FillRect(Rectangle rect)
		{
			glBegin(GL_QUADS);
			rect.x += root.x;
			rect.y += root.y;
			glVertex2i(rect.x, rect.y);
			glVertex2i(rect.x + rect.width, rect.y);
			glVertex2i(rect.x + rect.width, rect.y + rect.height);
			glVertex2i(rect.x, rect.y + rect.height);
			glEnd();
		}
		void FillAll()
		{
			glBegin(GL_QUADS);
			glVertex2i(root.x, root.y);
			glVertex2i(root.x + root.width, root.y);
			glVertex2i(root.x + root.width, root.y + root.height);
			glVertex2i(root.x, root.y + root.height);
			glEnd();
		}
	};

	class Component //:public MouseMsg
	{
	private:
		Rectangle parentBounds;
		Rectangle bounds;
		GLFWwindow* window = NULL;
		Graphics g;
		std::vector<Component*> childComponents;
	protected:
		void SetRootWindow(GLFWwindow* window)
		{
			this->window = window;
			g.SetWindow(window);
		}
		void SetParentBounds(Rectangle parentBounds)
		{
			this->parentBounds = parentBounds;
		}
		void AddAndMakeVisible(Component& component)
		{
			childComponents.push_back(&component);
			component.SetParentBounds(bounds);
			component.SetRootWindow(window);
		}
	public:
		//Component();
		//~Component();

		virtual void Resize()
		{
			//这个要重写，如果组件/窗口大小变了就会回调
		}

		void SetBounds(Rectangle bounds)
		{
			this->bounds = bounds;
			for (auto& child : childComponents)
			{
				child->SetParentBounds(bounds);
			}
		}
		Rectangle GetBounds()
		{
			return bounds;
		}


		void Repaint()
		{
			if (!window)return;
			glfwMakeContextCurrent(window);
			//g.SetWindow(window);
			g.SetBounds(parentBounds + bounds);
			Paint(g);
			for (auto& child : childComponents)
			{
				child->Repaint();
			}
			glfwSwapBuffers(window);
		}
		virtual void Paint(Graphics& g)
		{
			DBG("你应该继承Component然后重写Paint\n");
		}

		//virtual void MouseMove(Pos pos);
		//virtual void MouseDown(Pos pos, MouseButton button);
		//virtual void MouseUp(Pos pos, MouseButton button);
	};

}