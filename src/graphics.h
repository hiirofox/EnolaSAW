#pragma once
#include "glfw\glfw3.h"
#include "type.h"
#define _USE_MATH_DEFINES
#include <math.h>
namespace Enola
{
	class Graphics {
	private:
		GLFWwindow* window = NULL;
		Rectangle windowRect;
		Rectangle myBounds;
	public:
		void SetGLFWwindow(GLFWwindow* pwh)
		{
			window = pwh;
		}
		void ResizeProc(GLFWwindow* pwh, Rectangle& windowRect, Rectangle& myBounds)
		{
			this->windowRect = windowRect;
			this->myBounds = myBounds;
		}
		void UpdataRectangle()
		{
			glOrtho(0, windowRect.w, windowRect.h, 0, -0.01, 0.01);
			glViewport(myBounds.x, myBounds.y, myBounds.w, myBounds.h);
		}

		void SetColor(int rgba)
		{
			int r = (rgba >> 24) & 0xFF;
			int g = (rgba >> 16) & 0xFF;
			int b = (rgba >> 8) & 0xFF;
			int a = rgba & 0xFF;
			glColor4ub(r, g, b, a);
		}

		void Clear(int rgba)
		{
			SetColor(rgba);
			FillRectangle(0, 0, myBounds.w, myBounds.h);
		}

		void DrawLine(float x1, float y1, float x2, float y2, float thickness = 1.0f)
		{
			glLineWidth(thickness);
			glBegin(GL_LINES);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glEnd();
		}

		void DrawCircle(float x, float y, float radius, float thickness = 1.0f)
		{
			glLineWidth(thickness);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 360; i++) {
				float angle = i * M_PI / 180;
				glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
			}
			glEnd();
		}

		void FillCircle(float x, float y, float radius)
		{
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x, y);
			for (int i = 0; i <= 360; i++) {
				float angle = i * M_PI / 180;
				glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
			}
			glEnd();
		}

		void FillEllipse(float x, float y, float rx, float ry)
		{
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x, y);
			for (int i = 0; i <= 360; i++) {
				float angle = i * M_PI / 180;
				glVertex2f(x + cos(angle) * rx, y + sin(angle) * ry);
			}
			glEnd();
		}

		void DrawRectangle(float x1, float y1, float x2, float y2, float thickness = 1.0f)
		{
			glLineWidth(thickness);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x1, y1);
			glVertex2f(x2, y1);
			glVertex2f(x2, y2);
			glVertex2f(x1, y2);
			glEnd();
		}

		void FillRectangle(float x1, float y1, float x2, float y2)
		{
			glBegin(GL_QUADS);
			glVertex2f(x1, y1);
			glVertex2f(x2, y1);
			glVertex2f(x2, y2);
			glVertex2f(x1, y2);
			glEnd();
		}

	};
}