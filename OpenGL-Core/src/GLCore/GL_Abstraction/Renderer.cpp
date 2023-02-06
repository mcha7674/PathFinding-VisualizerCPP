#include "glpch.h"
#include "Renderer.h"

Renderer::Renderer()
{
    m_model = glm::mat4(1.0f);
 
}

void Renderer::setUniforms(const glm::mat4& model, Shader& shader)
{
    
    shader.SetUniformMatrix4fv("model", model);

}

// static function because we want it specific to this file (translation unit)
void GLClearError()
{   // purpose of funciton - clear all the errors (Get all of them
    while (glGetError() != GL_NO_ERROR);
}
// Boolean function to work with ASSERT
// additional parameters are for printing out the error with useful info
bool GLLogCall(const char* function, const char* file, int line)
{   // print out all caught errors
    while (GLenum error = glGetError()) // will run as long as error var is true (not 0)
    { // print out error with useful info given by parameters
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "[OpenGL Error] (error code: " << error << ")\nCulprit: '" << function <<
            "'\nFile location: " << file << "\nLine: " << line << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        return false;
    }
    return true;
}

void Renderer::DrawCircle(const VertexArray& va, Shader& shader, Transform& transform, float numVertices)
{
    shader.use();
    va.Bind(); // dont need to bind vb since it is attached to vertex array
    // Calculate the model matrix
    CALCULATE_MODEL_MATRIX();
    // set shader uniforms
    setUniforms(m_model, shader);
    GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices));
}

void Renderer::DrawRect(const VertexArray& va, const IndexBuffer& ib, Shader& shader, Transform &transform) 
{
    shader.use();
    va.Bind(); // dont need to bind vb since it is attached to vertex array
    ib.Bind();
    // Calculate the model matrix
    CALCULATE_MODEL_MATRIX();
    // set shader uniforms
    setUniforms(m_model, shader);
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), ib_GLtype, 0));
}


void Renderer::DrawPoints(const VertexArray& va, Shader& shader, Transform& transform, unsigned int count)
{
    shader.use();
    va.Bind();
    CALCULATE_MODEL_MATRIX();
    setUniforms(m_model, shader);
    glPointSize(2);
    GLCall(glDrawArrays(GL_POINTS, 0, count));
}

void Renderer::DrawLineStrip(const VertexArray& va, Shader& shader, Transform& transform, unsigned int count)
{
    shader.use();
    va.Bind();
    CALCULATE_MODEL_MATRIX();
    setUniforms(m_model, shader);
    //glLineWidth(5);
    GLCall(glDrawArrays(GL_LINE_STRIP, 0, count));
}



void Renderer::DrawTri(const VertexArray& va, const Shader& shader)
{
    shader.use();
    va.Bind(); // dont need to bind vb since it is attached to vertex array
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 3)); // 2nd arg: starting index. 3rd arg: how many vertices
}

void Renderer::DrawCube_noIb(const VertexArray& va, const Shader& shader)
{
    shader.use();
    va.Bind(); // dont need to bind vb since it is attached to vertex array
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36)); // 2nd arg: starting index. 3rd arg: how many vertices
}



void Renderer::Clear(bool depthBuffer)
{
    if (!depthBuffer)
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    else
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}


