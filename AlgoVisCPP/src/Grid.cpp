#include "Grid.h"

// Line strip Vertices
static float Vertices[] = {
	// positions    
	 0.0f,  0.0f, 0.0f,   // left coordinate
	 1.0f,  1.0f, 0.0f,   // right coordinate
};

Grid::Grid(Renderer& renderer, glm::mat4 ViewProjectionMatrix)
	:renderer(renderer), ViewProjectionMatrix(ViewProjectionMatrix)
{
	grid_shader = Shader::FromGLSLTextFiles(
		"assets/shaders/grid.vert.glsl",
		"assets/shaders/grid.frag.glsl"
	);
	// INIT VBO and IBO
	vb = std::make_unique<VertexBuffer>(Vertices, sizeof(Vertices)); // init vertex buffer (Gen, Bind, Data Implement)
	// Create layout
	layout.Push<float>(3); // 3 floats per vertex for position
	va.AddBuffer((*vb), layout);
	// Adjust the Scaling for the Colored Cell Quads
	fillQuad.trans.setScale( {0.88f,0.88f,0.0f} );
	// Init GridProperties object
	gridProps = std::make_shared<GridProperties>();
	setGridColor(gridProps->gridColor);
}


// init Grid - fill grid with defaults with correct size.
void Grid::Init(int row, int col) {
	grid = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col, Cell()));
	gridProps->height = row;
	gridProps->width = col;
}

// Draw Grid - Draws grid using line strips.
void Grid::DrawGrid() {
	// Draw Grid Lines //
	grid_shader->use();
	grid_shader->SetUniformMatrix4fv("viewProjection", ViewProjectionMatrix);
	Transform transV;
	transV.setScale({ 0.0f, (float)gridProps->height, 1.0f });
	// vertical lines
	for (int x = 0; x <= gridProps->width; x++) {
		transV.setPosition({ (float)x, 0.0f, 0.0f });
		renderer.DrawLineStrip(va, *(grid_shader), transV, 2);
	}
	Transform transH;
	transH.setScale({ (float)gridProps->width, 0.0f, 1.0f });
	// horizontal lines
	for (int y = 0; y <= gridProps->height; y++) {
		transH.setPosition({ 0.0f, (float)y, 0.0f });
		renderer.DrawLineStrip(va, *(grid_shader), transH, 2);
	}
	// Iterate Through and Fill grid with colored Quads where needed //
	fillQuad.quad_shader->use();
	fillQuad.quad_shader->SetUniformMatrix4fv("viewProjection", ViewProjectionMatrix);
	// Only Execute code below if both start and Endpoint has been set and algorithm chosen
	for (int i = 0; i < gridProps->width; i++) {
		for (int j = 0; j < gridProps->height; j++) {
			// Check if start and end, if so, render colored cell
			if (grid[j][i].isStart) {
				fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)i + 0.05f, (float)j + 0.05f, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid[j][i].isEnd) {
				fillQuad.setColor(1.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)i + 0.05f, (float)j + 0.05f, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else {
				switch (grid[j][i].m_State)
				{
				case VISITED:
					fillQuad.setColor(0.0f, 0.0f, 1.0f, 1.0f);
					fillQuad.trans.setPosition({ (float)i + 0.05f, (float)j + 0.05f, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case VISITING:
					fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
					fillQuad.trans.setPosition({ (float)i + 0.05f, (float)j + 0.05f, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case UNVISITED:
					// if unvisited do not draw a colored quad!
				default:
					break;
				}
			}
			
		}
	}
}

// Function to mark cells
void Grid::visitCell(int x, int y, bool isStart, bool isEnd)
{
	// If visiting cell is the start/end, mark it down in cell properties and mark as visited
	if (isStart) grid[x][y].isStart = true;
	else if (isEnd) grid[x][y].isEnd = true;

	if (grid[x][y].m_State == UNVISITED) {
		grid[x][y].m_State = VISITED;
	}
	else if (grid[x][y].m_State == VISITED) {
		grid[x][y].m_State == VISITING;
	}
}

void Grid::resetGrid()
{
	// Rest Relevant Grid Variables
	//
	// Mark all Cells as unvisited
	for (int i = 0; i < gridProps->width; i++) {
		for (int j = 0; j < gridProps->height; j++) {
			grid[j][i].m_State = UNVISITED;
		}
	}
}

void Grid::setGridColor(float r, float g, float b, float a)
{
	grid_shader->use();
	grid_shader->SetUniformVec4fv("u_Color", glm::vec4(r, g, b, a));
}
void Grid::setGridColor(glm::vec4 rgba)
{
	grid_shader->use();
	grid_shader->SetUniformVec4fv("u_Color", rgba);
}
