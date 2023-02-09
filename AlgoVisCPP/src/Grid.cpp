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
	fillQuad.trans.setScale( {1.0f,1.0f,0.0f} );
	// Init GridProperties object
	gridProps = std::make_shared<GridProperties>();
	setGridColor(gridProps->gridColor);
}


// init Grid - fill grid with defaults with correct size.
void Grid::Init(int row, int col) {
	grid = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col, Cell() ));
	// Fill Coordinates
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			grid[i][j].coords = {i, j};
			grid[i][j].parentCell = {i, j};
		}
	}
	gridProps->height = row;
	gridProps->width = col;
	
}

// Draws the Grid Lines
void Grid::DrawGridLines()
{
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
}

// Draw Grid - Fills in Grid.
void Grid::DrawGrid() {
	// Iterate Through and Fill grid with colored Quads where needed //
	fillQuad.quad_shader->use();
	fillQuad.quad_shader->SetUniformMatrix4fv("viewProjection", ViewProjectionMatrix);
	// Only Execute code below if both start and Endpoint has been set and algorithm chosen
	for (int i = 0; i < gridProps->height; i++) {
		for (int j = 0; j < gridProps->width; j++) {
			// Check if start and end, if so, render colored cell
			if (grid[i][j].m_Type == cellType::START) {
				fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid[i][j].m_Type == cellType::END) {
				fillQuad.setColor(1.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid[i][j].m_Type == cellType::WALL) {
				fillQuad.setColor(0.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid[i][j].m_Type == cellType::PATH) {
				fillQuad.setColor(1.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else {
				switch (grid[i][j].m_State)
				{
				case cellState::VISITED:
					fillQuad.setColor(0.3f, 0.3f, 1.0f, 0.8f);
					fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case cellState::VISITING:
					fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
					fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case cellState::UNVISITED: // if unvisited, no need to render a quad.
				default:
					break;
				}
			}
			
		}
	}
}


void Grid::setCellState(int row, int col, cellState state)
{
	grid[row][col].m_State = state;
}

void Grid::setCellType(int row, int col, cellType type)
{
	std::pair<int, int> currCoord = { row, col };
	// only set a WALL type if current cell is not start or End cell
	switch (type)
	{
	case cellType::START:
		if (gridProps->endCoord == currCoord) break;
		gridProps->startCoord = { row, col };
		grid[row][col].m_Type = type;
		grid[row][col].m_State = cellState::VISITED;
		
		break;
	case cellType::END:
		if (gridProps->startCoord == currCoord) break;
		gridProps->endCoord = { row, col };
		grid[row][col].m_Type = type;
		break;
	case cellType::WALL:
		if (gridProps->startCoord == currCoord || gridProps->endCoord == currCoord) break;
		grid[row][col].m_Type = type;
		break;
	case cellType::PATH:
		grid[row][col].m_Type = type;
		break;
	default:
		break;
	}
}

void Grid::setCellParent(int r0, int c0, int r, int c)
{
	grid[r][c].parentCell = { r0, c0 };
}

void Grid::resetGrid()
{
	for (int i = 0; i < gridProps->height; i++) {
		for (int j = 0; j < gridProps->width; j++) {
			grid[i][j].reset(); // reset each cell
		}
	}
	// reset Grid Properties
	gridProps->reset();
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
