#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"
#include <iostream>
#include <vector>
#include "Box.h"
#include "Circle.h"

const int width = 640;
const int height = 480;

float* pixels = new float[width*height * 3];

class Draw {
public:
	virtual void draw() {

	}
};

template<class T>
class Geometric: public Draw {
public:
	void draw() {
		T draw_shape;
		draw_shape.draw();
	}
};

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.

void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	if (i0 == i1) {
		for (int j = j0; j < j1; j++)
			drawPixel(i0, j, red, green, blue);
		return;
	}
	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}

void drawSquare(const int& edge, const int& i0, const int& j0, const float& red, const float& green, const float& blue)
{
	drawLine(i0, j0, i0 + edge, j0, 0.0f, 0.0f, 0.0f);
	drawLine(i0 + edge, j0, i0 + edge, j0 + edge, 0.0f, 0.0f, 0.0f);
	drawLine(i0, j0 + edge, i0 + edge, j0 + edge, 0.0f, 0.0f, 0.0f);
	drawLine(i0, j0, i0, j0 + edge, 0.0f, 0.0f, 0.0f);
	/*arroundCircle(i0 + 25, j0 + 25, 1.0f, 0.0f, 0.0f, window);*/
}

void drawCircle(const int& i0, const int& j0, const float& red, const float& green, const float& blue) {
	int r = 50;

	for (int i = i0 - r; i < i0 + r; i++) {
		for (int j = j0 - r; j < j0 + r; j++) {
			int n = (i - i0)*(i - i0) + (j - j0)*(j - j0) - r*r;
			int n2 = (i - i0)*(i - i0) + (j - j0)*(j - j0) - (r - 3)*(r - 3) + 6*r - 9;
			if (n < 0 && n2>0) {
				drawPixel(i, j, 1.0f, 0.0f, 0.0f);
			}
		}
	}
}

void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

													//for (int i = 0; i<width*height; i++) {
													//	pixels[i * 3 + 0] = 1.0f; // red 
													//	pixels[i * 3 + 1] = 1.0f; // green
													//	pixels[i * 3 + 2] = 1.0f; // blue
													//}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);

	// drawing a line
	//TODO: anti-aliasing

	std::vector<Draw*> obj_list;
	obj_list.push_back(new Geometric<Box>);
	obj_list.push_back(new Geometric<Circle>);
	for (auto itr : obj_list)
		itr->draw();

}

int main(void)
{
	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}
