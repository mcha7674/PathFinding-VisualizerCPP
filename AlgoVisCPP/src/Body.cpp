#include "Body.h"

Body::Body(float rad,float m, unsigned int numSides)
	:radius(rad), mass(m)
{
	texture = nullptr;
	NumberOfSides = numSides;
	// Create The Circle vertices
	InitCircle(NumberOfSides, radius);
}


void Body::setColor(float r, float g, float b, float a)
{
	circleColor = glm::vec4(r, g, b, a);
	Circle_shader->use();
	Circle_shader->SetUniformVec4fv("u_Color", circleColor);
}


void Body::setAlpha(const float alphaVal)
{
	circleColor.w = alphaVal;
	Circle_shader->use();
	Circle_shader->SetUniformVec4fv("u_Color", circleColor);
	
}

void Body::setTexture(const char* texturePath)
{
	texture = new Texture(texturePath);
	Circle_shader->use();
	Circle_shader->SetUniform1i("enableTexture", 1); // 2nd arg is the slot number
	Circle_shader->SetUniform1i("texture", 0); // 2nd arg is the slot number
}


