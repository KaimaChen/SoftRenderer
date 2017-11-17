#pragma once

class DemoScene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void ListenMouse(int mouseX, int mouseY, int buttons) {}
	virtual void ListenMiddleClick(int mouseX, int mouseY) {}
};