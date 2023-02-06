#pragma once
#include "Base_Models/Circle.h"

struct Body: public Circle
{
    // PARAMETERIZED CONSTRUCTOR
    Body(float radius,float m, unsigned int numSides = 100);

    virtual void setColor(float r, float g, float b, float a) override;
    void setAlpha(const float alphaVal);
    void setTexture(const char* texturePath);

    glm::vec4 circleColor;
    Transform body_Transform;
    Texture *texture;

    float mass; // in solar masses
    float radius; // In Sun Radii
};


