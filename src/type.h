#pragma once
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
}