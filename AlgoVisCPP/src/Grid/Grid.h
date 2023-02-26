#pragma once
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>
#include "GridProperties.h"
#include "GridRender.h"



class Grid
{
public:
	Grid(int row, int col, glm::mat4 ViewProjMatrix);
	~Grid() = default;
	void Init(int row, int col);
	void Draw(std::pair<float, float>& origin);
	void FillGrid(std::pair<float, float>& origin);
	void Resize(int row, int col);
	void UpdateViewProjection(glm::mat4 newVPMatrix);
	// Cell Setters
	inline void SetCellState(int r, int c, cellState state) { grid[r][c].m_State = state; }
	// Grid Setters
	void SetGridColor(glm::vec4 rgba);
	// Grid Getters
	GridProperties GetGridProperties() { return gridProps; };
private:
	GridRender gridRenderer;
	GridProperties gridProps;
	std::vector<std::vector<Cell>> grid;	
	Color colors;
};




