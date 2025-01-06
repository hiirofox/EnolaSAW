#pragma once

#include "glfw\glfw3.h"
#include "dbg.h"

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

struct Rectangle
{
	int x;
	int y;
	int width;
	int height;
};

class Graphics
{
private:
public:
};

class Component : public MouseMsg
{
private:
	Rectangle bounds;
	std::vector<Component> childComponents;
public:
	Component();
	~Component();

	void SetBounds(int x, int y, int width, int height);
	void SetBounds(Rectangle bounds);
	Rectangle GetBounds();

	void AddAndMakeVisible(Component component);
	void RemoveChild(Component component);

	void Repaint();
	void Paint(Graphics& g);

	virtual void MouseMove(Pos pos);
	virtual void MouseDown(Pos pos, MouseButton button);
	virtual void MouseUp(Pos pos, MouseButton button);
};