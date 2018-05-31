#pragma once


#ifndef LAB471_CAMERA_H_INCLUDED
#define LAB471_CAMERA_H_INCLUDED

#include <stack>
#include <memory>
#include "GLSL.h"
#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

class camera
{
public:
	glm::vec3 pos, rot;
	int up, down, right, left;
	camera()
	{
		up = down = right = left = 0;
		pos = rot = glm::vec3(0, 0, 0);
	}
	glm::mat4 process()
	{
		double ftime = get_last_elapsed_time();
		float speed = 0;
		if (up == 1)
		{
			speed = 1 * ftime * 5;
		}
		else if (down == 1)
		{
			speed = -1 * ftime * 5;
		}
		float yangle = 0;
		if (left == 1)
			yangle = -1 * ftime;
		else if (right == 1)
			yangle = 1 * ftime;
		rot.y += yangle;
		glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
		glm::vec4 dir = glm::vec4(0, 0, speed, 1);
		dir = dir * R;
		pos += glm::vec3(dir.x, dir.y, dir.z);
		glm::mat4 T = glm::translate(glm::mat4(1), pos);
		return R * T;
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








#endif // LAB471_CAMERA_H_INCLUDED