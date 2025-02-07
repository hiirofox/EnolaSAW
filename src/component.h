#pragma once

#include "glfw\glfw3.h"
#include "dbg.h"

namespace Enola
{
	class Component
	{
	private:
		std::vector<std::unique_ptr<Component>> childComponents;
	protected:
		void AddAndMakeVisable(Component& p)
		{
			childComponents.emplace_back(&p);
			//todo
		}
	public:
		virtual void Resize(int w, int h)
		{
		}
		virtual void Paint(GLFWwindow* pwh)
		{
		}
	};
}