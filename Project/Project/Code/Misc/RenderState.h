#pragma once

enum RenderMode
{
	WireFrame,
	Shading,
};

enum CullFace
{
	CullFront,
	CullBack,
	CullFrontAndBack,
};

enum ClockDirection
{
	CW,
	CCW,
};

enum BlendFunc
{
	SrcAlpha,
	OneMinusSrcAlpha,
};