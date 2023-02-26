#include "GridRender.h"


GridRender::GridRender(glm::mat4 ViewProjectionMatrix)
	:m_ViewProjectionMatrix(ViewProjectionMatrix)
{
	// Rendering Initializations
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

GridRender::~GridRender() 
{ 
	grid_shader->unuse(); 
	delete grid_shader; 
	delete vb; 
}

void GridRender::DrawGrid(const GridProperties& gridProps, std::pair<float, float> &origin)
{
	// Draw Grid Lines //
	grid_shader->use();
	grid_shader->SetUniformMatrix4fv("viewProjection", m_ViewProjectionMatrix);
	// vertical lines
	Transform transV;
	transV.setScale({ 0.0f, (float)gridProps.height, 1.0f });
	for (float x = 0; x <= (float)gridProps.width; x++) {
		transV.setPosition({ x + origin.first, origin.second, 0.0f});
		m_renderer.DrawLineStrip(*(va), *(grid_shader), transV, 2);
	}
	// horizontal lines
	Transform transH;
	transH.setScale({ (float)gridProps.width, 0.0f, 1.0f });
	for (float y = 0; y <= (float)gridProps.height; y++) {
		transH.setPosition({ origin.first, y + origin.second, 0.0f });
		m_renderer.DrawLineStrip(*(va), *(grid_shader), transH, 2);
	}
}

void GridRender::FillCell(int row, int col, glm::vec4 fillColor, std::pair<float, float>& origin)
{
	m_fillQuad.quad_shader->use();
	m_fillQuad.quad_shader->SetUniformMatrix4fv("viewProjection", m_ViewProjectionMatrix);
	// Render Filled Cell
	m_fillQuad.setColor(fillColor);
	m_fillQuad.trans.setPosition({ (float)row + origin.first, (float)col + origin.second, 0.0f });
	m_renderer.DrawRect(m_fillQuad.va, *(m_fillQuad.ib), *(m_fillQuad.quad_shader), m_fillQuad.trans);
}

void GridRender::SetGridColor(float r, float g, float b, float a)
{
	grid_shader->use();
	grid_shader->SetUniformVec4fv("u_Color", glm::vec4(r, g, b, a));
}
void GridRender::SetGridColor(glm::vec4 rgba)
{
	grid_shader->use();
	grid_shader->SetUniformVec4fv("u_Color", rgba);
}

void GridRender::SetViewProjMatrix(glm::mat4 viewProjMatrix)
{
	m_ViewProjectionMatrix = viewProjMatrix;
}