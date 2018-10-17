#pragma once


#ifndef LAB471_DISPLAYCASE_H_INCLUDED
#define LAB471_DISPLAYCASE_H_INCLUDED

#include <stack>
#include <memory>
#include "GLSL.h"
#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

class DisplayCase
{
public:

	glm::vec3 rot;
	int a, d;

	DisplayCase() {
		a = d = 0;
		rot = glm::vec3(0.0);
	}

	glm::mat4 process() {		// allow rotation of globe
		double ftime = get_last_elapsed_time();
		float yangle = 0;
		if (d == 1)
			yangle = -1 * ftime * 1;
		else if (a == 1)
			yangle = 1 * ftime * 1;
		rot.y += yangle;
		glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));

		return R;
	}

	double get_last_elapsed_time()
	{
		static double lasttime = glfwGetTime();
		double actualtime = glfwGetTime();
		double difference = actualtime - lasttime;
		lasttime = actualtime;
		return difference;
	}

};








#endif // LAB471_DISPLAYCASE_H_INCLUDED