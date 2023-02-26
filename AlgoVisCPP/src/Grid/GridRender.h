#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include "../Color.h"
#include "../Base_Models/Quad.h"
#include "GridProperties.h"

// Line strip Vertices
static const float lineVertices[] = {
	// positions    
	 0.0f,  0.0f, 0.0f,   // left coordinate
	 1.0f,  1.0f, 0.0f,   // right coordinate
};

class GridRender
{
public:
	GridRender(glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f));
	~GridRender();
	inline VertexArray* getVA() { return va; }
	inline Shader* getShader() { return grid_shader; }
	inline int vertexCount() { return 2; }

	void DrawGrid(const GridProperties &gridProps, std::pair<float, float>& origin);
	void FillCell(int row, int col, glm::vec4 fillColor, std::pair<float, float>& origin);
	void SetViewProjMatrix(glm::mat4 viewProjMatrix);
	void SetGridColor(float r, float g, float b, float a = 1.0f);
	void SetGridColor(glm::vec4 rgba);
private:
	glm::mat4 m_ViewProjectionMatrix;
	Renderer m_renderer;
	Quad m_fillQuad;
	Color m_colors;

	Shader* grid_shader;
	VertexArray *va; 
	VertexBuffer* vb;
	VertexBufferLayout layout;
};
