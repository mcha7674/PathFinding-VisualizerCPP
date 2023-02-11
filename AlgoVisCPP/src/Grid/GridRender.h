#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>

// Line strip Vertices
static const float lineVertices[] = {
	// positions    
	 0.0f,  0.0f, 0.0f,   // left coordinate
	 1.0f,  1.0f, 0.0f,   // right coordinate
};

class GridRender
{
public:
	GridRender()
	{
		grid_shader = Shader::FromGLSLTextFiles(
			"assets/shaders/grid.vert.glsl",
			"assets/shaders/grid.frag.glsl"
		);
		// INIT VBO and IBO
		va = new VertexArray();
		vb = new VertexBuffer(lineVertices, sizeof(lineVertices)); // init vertex buffer (Gen, Bind, Data Implement)
		// Create layout
		layout.Push<float>(3); // 3 floats per vertex for position
		va->AddBuffer((*vb), layout);
	}
	~GridRender() { grid_shader->unuse(); delete grid_shader; delete vb; }
	inline VertexArray* getVA() { return va; }
	inline Shader* getShader() { return grid_shader; }
	inline int vertexCount() { return 2; }
private:
	Shader* grid_shader;
	VertexArray *va; // (GenVertexArray)
	VertexBuffer* vb; // (Gen, Bind, Data Implement)
	VertexBufferLayout layout;
};
