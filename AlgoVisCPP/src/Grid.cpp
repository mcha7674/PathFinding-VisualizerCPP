#include "Grid.h"

static float Vertices[] = {
	// positions    
	 0.0f,  0.0f, 0.0f,   // left coordinate
	 1.0f,  1.0f, 0.0f,   // right coordinate
};

Grid::Grid(Renderer& renderer, glm::mat4 ViewProjectionMatrix)
	:renderer(renderer), ViewProjectionMatrix(ViewProjectionMatrix)
{
	cell_shader = Shader::FromGLSLTextFiles(
		"assets/shaders/Cell.vert.glsl",
		"assets/shaders/Cell.frag.glsl"
	);
	// INIT VBO and IBO
	vb = new VertexBuffer(Vertices, sizeof(Vertices)); // init vertex buffer (Gen, Bind, Data Implement)
	// Create layout
	layout.Push<float>(3); // 3 floats per vertex for position
	va.AddBuffer((*vb), layout);
	cell_shader->use();
	cell_shader->SetUniformMatrix4fv("viewProjection", ViewProjectionMatrix);
}

// init Grid - fill grid with defaults with correct size.
void Grid::Init(int row, int col) {
	grid = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col, Cell()));
	height = row;
	width = col;
}
// Draw Grid - using passed in params, render the grid
void Grid::DrawGrid() {
	Transform transV;
	transV.setScale({ 0.0f, (float)height, 1.0f });
	// vertical lines
	for (int x = 0; x <= width; x++) {
		transV.setPosition({ (float)x, 0.0f, 0.0f });
		renderer.DrawLineStrip(va, *(cell_shader), transV, 2);
	}
	Transform transH;
	transH.setScale({ (float)width, 0.0f, 1.0f });
	// horizontal lines
	for (int y = 0; y <= height; y++) {
		transH.setPosition({ 0.0f, (float)y, 0.0f });
		renderer.DrawLineStrip(va, *(cell_shader), transH, 2);
	}
	
}
