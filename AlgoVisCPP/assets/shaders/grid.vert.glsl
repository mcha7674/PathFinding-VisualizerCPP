#version 450 core

layout (location = 0) in vec3 a_Position;

uniform mat4 model;
uniform mat4 viewProjection;


void main()
{
	gl_Position =  viewProjection * model * vec4(a_Position, 1.0f);
}