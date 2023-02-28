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
	void SetCellType(int r, int c, cellType type);
	// Cell Getters
	inline int GetCellID(int r, int c) { return grid[r][c].id; }
	inline std::pair<int, int> GetCellCoord(int id) { return cellHash[id]; }
	inline cellType GetCellType(int r, int c) { return grid[r][c].m_Type; }
	inline cellType GetCellType(int cellID) { return grid[cellHash[cellID].first][cellHash[cellID].second].m_Type; }
	inline cellState GetCellState(int r, int c) { return grid[r][c].m_State; }
	// Grid Setters
	void SetGridColor(glm::vec4 rgba);
	// Grid Getters
	GridProperties GetGridProperties() { return gridProps; };
	inline int GetHeight() { return gridProps.height; }
	inline int GetWidth() { return gridProps.width; }
	inline std::pair<int, int> GetStartCoord() { return gridProps.startCoord; }
	inline bool isStartAndEndSet() { return gridProps.endPointSet && gridProps.startPointSet; }
	inline bool isEndSet() { return gridProps.endPointSet ; }
	inline bool isStartSet() { return gridProps.startPointSet; }
private:
	GridRender gridRenderer;
	GridProperties gridProps;
	std::vector<std::vector<Cell>> grid;	
	std::unordered_map<int, std::pair<int, int>> cellHash;
	Color colors;
};




