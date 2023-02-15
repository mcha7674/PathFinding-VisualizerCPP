#include "Grid.h"

Grid::Grid(int row, int col, glm::mat4 ViewProjMatrix)
{
	Init(row, col, ViewProjMatrix);
}

void Grid::Init(int row, int col, glm::mat4 ViewProjMatrix)
{
	ViewProjectionMatrix = ViewProjMatrix;
	gridProps = std::make_shared<GridProps>();
	gridProps->height = row;
	gridProps->width = col;
	grid = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col, Cell()));
	// Fill Coordinates
	int ID = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// update ID and add coordinates to hash
			grid[i][j].ID = ID;
			cellHash[ID] = { i, j };
			ID++;
		}
	}
}

void Grid::RenderGrid()
{
	fillQuad.quad_shader->use();
	fillQuad.quad_shader->SetUniformMatrix4fv("viewProjection", ViewProjectionMatrix);
	// Only Execute code below if both start and Endpoint has been set and algorithm chosen
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			// Check if start and end, if so, render colored cell
			if (getCellType(i, j) == cellType::START) {
				fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (getCellType(i, j) == cellType::END) {
				fillQuad.setColor(1.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (getCellType(i, j) == cellType::WALL) {
				fillQuad.setColor(0.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (getCellType(i, j) == cellType::PATH) {
				fillQuad.setColor(1.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else {
				switch (getCellState(i, j))
				{
				case cellState::VISITING: // Visiting will always be set first
					fillQuad.setColor(0.0f, 1.0f, 1.0f, 1.0f);
					fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case cellState::VISITED:
					fillQuad.setColor(0.3f, 0.3f, 1.0f, 0.8f);
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
void Grid::RenderGridLines()
{
	// Draw Grid Lines //
	gridRender.getShader()->use();
	gridRender.getShader()->SetUniformMatrix4fv("viewProjection", ViewProjectionMatrix);
	Transform transV;
	transV.setScale({ 0.0f, (float)getHeight(), 1.0f });
	// vertical lines
	for (int x = 0; x <= getWidth(); x++) {
		transV.setPosition({ (float)x, 0.0f, 0.0f });
		renderer.DrawLineStrip(*(gridRender.getVA()), *(gridRender.getShader()), transV, gridRender.vertexCount());
	}
	Transform transH;
	transH.setScale({ (float)getWidth(), 0.0f, 1.0f });
	// horizontal lines
	for (int y = 0; y <= getHeight(); y++) {
		transH.setPosition({ 0.0f, (float)y, 0.0f });
		renderer.DrawLineStrip(*(gridRender.getVA()), *(gridRender.getShader()), transH, gridRender.vertexCount());
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
		gridProps->startPointSet = true;
		// if Start node has been set already, reset it to a normal cell and set this new one
		if (gridProps->startPointSet) {
			grid[gridProps->startCoord.first][gridProps->startCoord.second].m_Type = cellType::NORMAL;
		}
		gridProps->startCoord = { row, col };
		grid[row][col].m_Type = type;
		grid[row][col].m_State = cellState::VISITED;
		break;
	case cellType::END:
		if (gridProps->startCoord == currCoord) break;
		gridProps->endPointSet = true;
		// if End node has been set already, reset it to a normal cell and set this new one
		if (gridProps->endPointSet) {
			grid[gridProps->endCoord.first][gridProps->endCoord.second].m_Type = cellType::NORMAL;
		}
		gridProps->endCoord = { row, col };
		grid[row][col].m_Type = type;
		break;
	case cellType::WALL:
		// dont make wall if trying to change the start or end nodes
		if (gridProps->startCoord == currCoord || gridProps->endCoord == currCoord) break;
		grid[row][col].m_Type = type;
		break;
	case cellType::PATH:
		grid[row][col].m_Type = type;
		std::cout << "Setting Cell " << row << "," << col << " To a PATH cell" << std::endl;
		break;
	case cellType::NORMAL:
		grid[row][col].m_Type = type;
		break;
	default:
		break;
	}
}


void Grid::clearBoard()
{
	for (int i = 0; i < gridProps->height; i++) {
		for (int j = 0; j < gridProps->width; j++) {
			if (grid[i][j].m_Type == cellType::START || grid[i][j].m_Type == cellType::END)
				continue;
			grid[i][j].reset(); // reset each cell
		}
	}
}

// Unvisit every visited grid and set path cells back to normal
void Grid::clearPath()
{
	for (int i = 0; i < gridProps->height; i++) {
		for (int j = 0; j < gridProps->width; j++) {
			if (getCellType(i, j) == cellType::PATH){
				setCellType(i, j, cellType::NORMAL); 
			}
			setCellState(i, j, cellState::UNVISITED);
		}
	}
}

// Hard reset EVERY cell to wipe the grid clean.
void Grid::reset()
{  
	gridProps->reset();
	for (int i = 0; i < gridProps->height; i++) {
		for (int j = 0; j < gridProps->width; j++) {
			grid[i][j].reset();
		}
	}
}

void Grid::setGridColor(float r, float g, float b, float a)
{
	gridRender.getShader()->use();
	gridRender.getShader()->SetUniformVec4fv("u_Color", glm::vec4(r, g, b, a));
}
void Grid::setGridColor(glm::vec3 rgb)
{
	gridRender.getShader()->use();
	gridRender.getShader()->SetUniformVec4fv("u_Color", glm::vec4(rgb, 1.0f));
}