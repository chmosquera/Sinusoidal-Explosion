
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>

// value_ptr for glm
#include "GLSL.h"
#include "Program.h"
#include "camera.h"
#include "displaycase.h"
#include "WindowManager.h"
#include "Shape.h"
#include "MatrixStack.h"

#define _USE_MATH_DEFINES
using namespace std;
using namespace glm;

// global variables
shared_ptr<Shape> shape;


class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	//camera and display case
	camera mycam;
	DisplayCase DC;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Toggles - sent to the shaders
	int PAUSE = 0, RESET = 0;
	int REVERSE = 1;

	// transitioning objects
	//float initPos[2], finalPos[2];
	glm::vec2 finalPos = glm::vec2(0.0);
	glm::vec2 initPos = glm::vec2(0.0);
	float transClock = 0;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;
	GLuint VertexArrayID_Points;

	// Data necessary to give our cylinder to OpenGL
	GLuint VertexBufferID, IndexBufferIDBox, NormalBufferID;
	GLuint VertexBufferID_Points;

	// helper functions for mouseCallback()
	float p2wX(int pX, int width) {

		float wX = pX / (width / 2.0) - 1.0;
		return wX;
	}
	float p2wY(int pY, int height) {

		// fix y-coord to be standard pixel coords (change sign)
		pY = 0 - pY;

		float wY = pY / (height / 2.0) + 1.0;
		return wY;
	}

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			mycam.up = 1;
		}
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		{
			mycam.up = 0;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			mycam.down = 1;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		{
			mycam.down = 0;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			mycam.right = 1;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		{
			mycam.right = 0;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			mycam.left = 1;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		{
			cout << "left" << endl;
			mycam.left = 0;
		}

		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			DC.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			DC.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			DC.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			DC.d = 0;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			if (PAUSE == 0) PAUSE = 1;
			else if (PAUSE == 1) PAUSE = 0;

			cout << "space hit!! Pause = " << PAUSE << endl;
		}
		if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			RESET = 1;
		}
		if (key == GLFW_KEY_R && action == GLFW_RELEASE)
		{
			RESET = 0;
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			if (REVERSE == -1) REVERSE = 1;
			else if (REVERSE == 1) REVERSE = -1;
		}
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{

		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

		if (action == GLFW_PRESS)
		{
			double x = 0, y = 0;
			glfwGetCursorPos(window, &x, &y);

			// let initPos be the previous finalPos
			initPos = finalPos;

			finalPos.x = p2wX(x, width);
			finalPos.y = p2wY(y, height);

			transClock = 0;
		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	// Read in meshes
	void initShape()
	{
		string resourceDirectory = "../resources";
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/bunny.obj");
		shape->resize();
		shape->init();

	}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{

		// init other geoms
		initShape();
	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/obj.vert", resourceDirectory + "/obj.frag", resourceDirectory + "/obj.geom");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertCol");
		prog->addUniform("camPos");
		prog->addUniform("explode");
		prog->addUniform("time");
		prog->addUniform("transClock");
		prog->addUniform("pointA");
		prog->addUniform("pointB");

	}

	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{

		// enable transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width / (float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width / (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones
		V = mycam.process();

		// all obj transformations
		glm::mat4  TransObj = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10)) * DC.process();
		M = TransObj;

		prog->bind();

		// update explode factor & timer
		static float explode = 0, time = 0.0;
		if (RESET == 1) {
			explode = 0;
		}
		if (PAUSE == 0) {
			if (REVERSE == 1) explode += 0.01;
			else if (REVERSE == -1) explode -= 0.01;
			time += 0.02;
		}
		cout << "pause: " << PAUSE << endl;
		if (explode < -0.1) {
			explode = -0.1;
		}

		// movement transition timer
		transClock += 0.008;			
		
		cout << "time: " << time << endl;
		// send the uniforms to the shaders
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform3fv(prog->getUniform("campos"), 1, &mycam.pos[0]);
		glUniform1f(prog->getUniform("explode"), explode);
		glUniform1f(prog->getUniform("time"), time);
		glUniform1f(prog->getUniform("transClock"), transClock);
		glUniform2fv(prog->getUniform("pointA"), 1, &initPos[0]);				// allows object to move
		glUniform2fv(prog->getUniform("pointB"), 1, &finalPos[0]);

		cout << "reverse: " << REVERSE << endl;

		shape->draw(prog);

		prog->unbind();

		glBindVertexArray(0);
	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
	and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
	may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();


	// Loop until the user closes the window.
	while (!glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
