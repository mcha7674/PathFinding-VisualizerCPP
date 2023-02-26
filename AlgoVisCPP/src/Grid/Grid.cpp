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
	for (int i = 0; i < row; i++) {
		grid.push_back({});
		for (int j = 0; j < col; j++) {
			grid[i].emplace_back(i,j);
		}
	}
}

void Grid::Draw(std::pair<float, float> &origin)
{
	gridRenderer.DrawGrid(gridProps, origin);
}

void Grid::FillGrid(std::pair<float, float>& origin)
{
	for (int i = 0; i < gridProps.height; i++)
	{
		float fill = true;
		for (int j = 0; j < gridProps.width; j++)
		{
			if (grid[i][j].m_Type == cellType::START) {
			}
			else if (grid[i][j].m_Type == cellType::END) {
			}
			else if (grid[i][j].m_Type == cellType::WALL) {
			}
			else if (grid[i][j].m_Type == cellType::PATH) {
			}
			else {
				switch (grid[i][j].m_State)
				{
				case cellState::VISITING:
					break;
				case cellState::VISITED:
					fill = true;
					break;
				case cellState::UNVISITED:
					fill = false;
					break;
				default:
					break;
				}
			}
			// Render Cell Quad
			if (fill) gridRenderer.FillCell(i, j, colors.VisitedCell, origin);
		}
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
