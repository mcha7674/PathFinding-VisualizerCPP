#pragma once
#include <GLAbstraction.h>

static float PI = 3.141592653589f;


struct Circle
{
    Shader* Circle_shader;
    Circle();
    ~Circle();

    virtual void setColor(float r, float g, float b, float a) = 0;
    void InitCircle(const unsigned int numberOfSides, const float radius);
    void fillCircleVertices(const float& r);

    /* VAO VB and VB Layout */
    VertexArray va; // (GenVertexArray)
    VertexBuffer* vb; // (Gen, Bind, Data Implement)
    //IndexBuffer* ib;
    VertexBufferLayout layout;

    std::vector <float> vertices;

    unsigned int NumberOfSides;
    unsigned int NumberOfVertices;
    //float radius;
};



