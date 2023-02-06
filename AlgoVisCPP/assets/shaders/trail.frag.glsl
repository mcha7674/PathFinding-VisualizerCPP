#version 450 core
layout(location = 0)out vec4 FragTrailColor;

uniform vec4 trailColor;

void main()
{
    FragTrailColor = trailColor;

}