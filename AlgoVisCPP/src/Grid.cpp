#include "Grid.h"

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
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			grid[x][y].pos.setPosition({ (float)x,(float)y,0.0 });
			renderer.DrawLineLoop(va, *(cell_shader), grid[x][y].pos, 4);
		}
	}
}