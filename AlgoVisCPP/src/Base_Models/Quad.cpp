#include "Quad.h"

Quad::Quad()
{
    // Init Shader
    quad_shader = Shader::FromGLSLTextFiles(
        "assets/shaders/quad.vert.glsl",
        "assets/shaders/quad.frag.glsl"
    );

    // INIT VBO and IBO
    vb = new VertexBuffer(vertices, sizeof(vertices)); // init vertex buffer (Gen, Bind, Data Implement)
    ib = new IndexBuffer(indices, 6);
    // Create layout
    layout.Push<float>(3); // 3 floats per vertex for position
    // Enables each Vertex Attribute Array and and create Attribute pointers
    va.AddBuffer((*vb), layout);

}

Quad::~Quad()
{
    va.UnBind();
    vb->UnBind();
    ib->UnBind();
    quad_shader->unuse();

    // delete mem allocation
    delete vb;
    delete ib;
    delete quad_shader;
    delete texture;

}

void Quad::setColor(float r, float g, float b, float a)
{
    quad_shader->use();
    quad_shader->SetUniformVec4fv("u_Color", glm::vec4(r, g, b, a));
}
void Quad::setColor(glm::vec4 color)
{
    // Color values should be normalized
    quad_shader->use();
    quad_shader->SetUniformVec4fv("u_Color", color);
}
