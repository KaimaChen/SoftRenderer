#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

void DrawPixel(int x, int y);
void DrawLine(int x1, int y1, int x2, int y2);
void DDADrawLine(int x1, int y1, int x2, int y2);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		//DrawPixel(400, 300);
		DrawLine(0, 0, 400, 400);

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}

void DrawPixel(int x, int y)
{
	float fx = (float)x / WIDTH;
	fx = fx * 2 - 1;
	float fy = (float)y / HEIGHT;
	fy = fy * 2 - 1;

	glBegin(GL_POINTS);
	glVertex3f(fx, fy, 0.0);
	glEnd();
}

void DrawLine(int x1, int y1, int x2, int y2)
{
	DDADrawLine(x1, y1, x2, y2);
}

//Digital Differential Analyzer (DDA) algorithm
void DDADrawLine(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = 0;
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	float xIncrement = dx / (float)steps;
	float yIncrement = dy / (float)steps;

	float x = x1;
	float y = y1;
	for (int v = 0; v < steps; v++)
	{
		x += xIncrement;
		y += yIncrement;
		DrawPixel(x, y);
	}
}