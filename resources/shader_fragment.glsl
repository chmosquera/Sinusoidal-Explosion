#version 330 core
out vec4 color;
in vec3 fcolor;
void main()
{
	
	color.xyz = fcolor;
	color.a = 1;

}
