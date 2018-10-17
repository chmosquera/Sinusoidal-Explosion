#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertCol;
layout(location = 2) in vec3 vertNor;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 vertex_normal;
out vec3 vcolor;

void main()
{
	gl_Position = P* V * M * vec4(vertPos, 1.0);
	vcolor = vertCol;
}
