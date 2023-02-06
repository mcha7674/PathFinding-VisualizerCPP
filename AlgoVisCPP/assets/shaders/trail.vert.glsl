#version 450 core
layout(location = 0) in vec3 trailPos;

uniform mat4 model;
uniform mat4 viewProjection;


void main() {
	// the gl_position is by default a vec4() so we need to cast our vec3 pos to a vec4. the w terms (4th ele) is set to 1 (unit vector).
	gl_Position = viewProjection * model * vec4(trailPos, 1.0);
}