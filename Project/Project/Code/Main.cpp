#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <ctime>
#include "Misc\Settings.h"
#include "GameMain.h"

using namespace std;

void DrawPixel(int x, int y);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Software Renderer", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	Init(DrawPixel);

	float totalFrames = 0.0f;
	float totalTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();		
		glClear(GL_COLOR_BUFFER_BIT);

		time_t start, end;
		start = clock();

		Update();

		end = clock();
		float diff = (float)(end - start) / CLOCKS_PER_SEC;
		++totalFrames;
		totalTime += diff;
		float fps = totalFrames / totalTime;
		//Debug::Log(fps);

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}

void DrawPixel(int x, int y)
{
	float fx = (float)x / SCREEN_WIDTH;
	fx = fx * 2 - 1;
	float fy = (float)y / SCREEN_HEIGHT;
	fy = fy * 2 - 1;

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex3f(fx, fy, 0.0);
	glEnd();
}