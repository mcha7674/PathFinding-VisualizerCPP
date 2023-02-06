#version 450 core

out vec4 o_Color;
in vec2 TexCoord;
uniform sampler2D texture1;
//uniform vec4 u_Color;

void main()
{
	o_Color = texture(texture1, TexCoord);
	//o_Color = u_Color;
}