#include "Grid.h"

Grid::Grid(int row, int col, glm::mat4 ViewProjMatrix)
{
	gridRenderer.SetViewProjMatrix(ViewProjMatrix);
	Init(row, col);
	// Init Grid FIll Quad params
	SetGridColor({ 0.0f,0.0f,0.0f,1.0f });
}

void Grid::Init(int row, int col)
{
	grid.clear();
	gridProps.SetDefault();
	gridProps.height = row;
	gridProps.width = col;
	// Fill Coordinates
	int id = 0;
	for (int i = 0; i < row; i++) {
		grid.push_back({});
		for (int j = 0; j < col; j++) {
			grid[i].emplace_back(i,j);
			cellHash[id] = { i, j };
			grid[i][j].id = id;
			id++;
		}
	}
}

void Grid::Draw(std::pair<float, float> &origin)
{
	gridRenderer.DrawGrid(gridProps, origin);
}

void Grid::FillGrid(std::pair<float, float>& origin)
{
	glm::vec4 chosenFillColor = { 0.0f,0.0f,0.0f,0.0f };
	for (int i = 0; i < gridProps.height; i++)
	{
		bool fill = true;
		for (int j = 0; j < gridProps.width; j++)
		{
			if (GetCellType(i, j) == cellType::START) {
				chosenFillColor = colors.StartCell;
			}
			else if (GetCellType(i, j) == cellType::END) {
				chosenFillColor = colors.EndCell;
			}
			else if (GetCellType(i, j) == cellType::WALL) {
				chosenFillColor = colors.WallCell;
			}
			else if (GetCellType(i, j) == cellType::PATH) {
				chosenFillColor = colors.PathCell;
			}
			else {
				switch (GetCellState(i, j))
				{
				case cellState::VISITING: 
					chosenFillColor = colors.VisitingCell;
					break;
				case cellState::VISITED:
					chosenFillColor = colors.VisitedCell;
					break;
				case cellState::UNVISITED:
					fill = false;
					break;
				default:
					break;
				}
			}
			// Render Cell Quad
			if (fill) gridRenderer.FillCell(i, j, chosenFillColor, origin);
		}
	}
}

void Grid::SetCellType(int row, int col, cellType type)
{
	std::pair<int, int> currCoord = { row, col };
	// only set a WALL type if current cell is not start or End cell
	switch (type)
	{
	case cellType::START:
		if (gridProps.endCoord == currCoord) break;
		// if Start node has been set already, reset it to a normal cell and set this new one
		gridProps.startPointSet = true;
		gridProps.startCoord = { row, col };
		grid[row][col].m_Type = type;
		grid[row][col].m_State = cellState::VISITED;
		break;
	case cellType::END:
		if (gridProps.startCoord == currCoord) break;
		// if End node has been set already, reset it to a normal cell and set this new one
		gridProps.endPointSet = true;
		gridProps.endCoord = { row, col };
		grid[row][col].m_Type = type;
		break;
	case cellType::WALL:
		// dont make wall if trying to change the start or end nodes
		if (gridProps.startCoord == currCoord || gridProps.endCoord == currCoord) break;
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

void Grid::SetGridColor(glm::vec4 rgba)
{
	gridRenderer.SetGridColor(rgba);
}

void Grid::UpdateViewProjection(glm::mat4 newVPMatrix)
{
	gridRenderer.SetViewProjMatrix(newVPMatrix);
}

void Grid::Resize(int row, int col)
{
	// Reset Relevant Grid Properties
	Init(row, col);
}
