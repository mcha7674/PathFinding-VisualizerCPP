#include "glpch.h"
#pragma once

#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "../Util/Shader.h"
#include "Transform.h"

using namespace GLCore::Utils;
// ERROR HANDLING MACROS
// C++ MACROS ASSERTION ->  compiler intrinsic (debugbreak wont work with other compilers , just msc)
#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG // IF DEBUGGING - GET ERROR AND PRINT OUT ERROR
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else // IF NOT DEBUGGING AND THIS IS THE REALEASR VERSION, GLCALL DOES NOTHING
#define GLCall(x) x
#endif

// ERROR HANDLING FUNCTIONS
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// MODEL MATRIX TRANSFORMATION MACROS
#define CALCULATE_MODEL_MATRIX() m_model = glm::mat4(1.0f); \
                                 m_model = glm::translate(m_model, transform.position); \
                                 m_model = glm::scale(m_model, transform.scale);

/* 
Renderer will handle draw calls. What do we need to draw??
- Vertex Array (which has vertex buffers bound to it)
- Index Buffer
- Shader
*/
class Renderer
{
public:
	glm::mat4 m_model;
public:
	Renderer();
	/* Set Uniforms for matrix transformation of objects */
	void setUniforms(const glm::mat4 &model, Shader &shader);
	/* Basic Draws */
	void DrawRect(const VertexArray &va, const IndexBuffer &ib, Shader &shader, Transform &transform);
	void DrawCircle(const VertexArray& va, Shader& shader, Transform& transform, float numVertices);
	void DrawPoints(const VertexArray& va, Shader& shader, Transform& transform, unsigned int count);
	void DrawLineStrip(const VertexArray& va, Shader& shader, Transform& transform, unsigned int count);


	//// draw triangles
	void DrawTri(const VertexArray &va, const Shader &shader);
	//// Draw Cubes
	void DrawCube_noIb(const VertexArray& va, const Shader& shader);

	// clear buffer bits
	void Clear(bool depthBuffer = false);

};