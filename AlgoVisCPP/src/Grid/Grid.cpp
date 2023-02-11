#include "Grid.h"

Grid::Grid(uint32_t row, uint32_t col)
{
	gridProps = std::make_shared<GridProps>();
	grid = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col, Cell()));
	// Fill Coordinates
	for (uint32_t i = 0; i < row; i++) {
		for (uint32_t j = 0; j < col; j++) {
			grid[i][j].coords = { i, j };
			grid[i][j].parentCell = { i, j };
		}
	}
	gridProps->height = row;
	gridProps->width = col;
}

void Grid::setCellState(uint32_t row, uint32_t col, cellState state)
{
	grid[row][col].m_State = state;
}

void Grid::setCellType(uint32_t row, uint32_t col, cellType type)
{
	std::pair<uint32_t, uint32_t> currCoord = { row, col };
	// only set a WALL type if current cell is not start or End cell
	switch (type)
	{
	case cellType::START:
		gridProps->startPointSet = true;
		if (gridProps->endCoord == currCoord) break;
		// if Start node has been set already, reset it to a normal cell and set this new one
		if (gridProps->startPointSet) {
			grid[gridProps->startCoord.first][gridProps->startCoord.second].m_Type = cellType::NORMAL;
		}
		gridProps->startCoord = { row, col };
		grid[row][col].m_Type = type;
		grid[row][col].m_State = cellState::VISITED;
		break;
	case cellType::END:
		gridProps->endPointSet = true;
		if (gridProps->startCoord == currCoord) break;
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
		break;
	case cellType::NORMAL:
		grid[row][col].m_Type = type;
		break;
	default:
		break;
	}
}

void Grid::setCellParent(uint32_t r0, uint32_t c0, uint32_t r, uint32_t c)
{
	grid[r][c].parentCell = { r0, c0 };
}

void Grid::clearBoard()
{
	for (int i = 0; i < gridProps->height; i++) {
		for (int j = 0; j < gridProps->width; j++) {
			grid[i][j].reset(); // reset each cell
		}
	}
	// reset Grid Properties
	gridProps.reset();
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
			setCellParent(i, j, i, j); // reset cell parents to themselves.
		}
	}
}