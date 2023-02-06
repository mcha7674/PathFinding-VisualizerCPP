#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 viewProjection;


void main()
{
	gl_Position =  viewProjection * model * vec4(a_Position, 1.0f);
	TexCoord = aTexCoord;
}