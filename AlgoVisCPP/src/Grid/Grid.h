#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>
#include "../Base_Models/Quad.h"
#include "GridRender.h"


enum class cellState { VISITED = 0, UNVISITED, VISITING };
enum class cellType { NORMAL = 0, START, END, WALL, PATH };

// Grid Cell Properties //
struct Cell
{
	Cell() { reset(); }
	void reset()
	{
		weight = 0;
		m_State = cellState::UNVISITED;
		m_Type = cellType::NORMAL;
		parentCell = coords;
	}
	std::pair<int, int> coords;
	std::pair<int, int> parentCell; // Keep track of cell visited by for shortest path
	int weight;
	cellType m_Type;
	cellState m_State;
};
// Grid Properties //
struct GridProps {
	int width = 0;
	int height = 0;
	glm::vec4 gridColor = { 0.0f,0.0f,0.0f,1.0f };

	bool startPointSet = false;
	bool endPointSet = false;
	std::pair<int, int>startCoord;
	std::pair<int, int>endCoord;
	GridProps() { reset(); }
	void reset() {
		startPointSet = false;
		endPointSet = false;
		startCoord = { 0, 0 };
		endCoord = { 0, 0 };
	}
};

class Grid
{
public:
	Grid(int row, int col, glm::mat4 ViewProjMatrix);
	~Grid() = default;
	// Reset Functions
	void clearBoard();
	void clearPath();
	void reset();
	// Getters
	inline Cell getCell(int r, int c) { return grid[r][c]; }
	inline cellState getCellState(int r, int c) { return grid[r][c].m_State; }
	inline cellType getCellType(int r, int c) { return grid[r][c].m_Type; }
	inline std::pair<int, int> getCellParent(int r, int c) { return grid[r][c].parentCell; }
	inline int getHeight() { return gridProps->height; }
	inline int getWidth() { return gridProps->width; }
	inline std::pair<int, int> getStartCoord() { return gridProps->startCoord; }
	std::vector<std::vector<Cell>> getGrid() { return grid; }	
	// Setters
	void setCellState(int row, int col, cellState state);
	void setCellType(int row, int col, cellType type);
	void setCellParent(int r0, int c0, int r, int c);
	inline void setGrid(std::vector<std::vector<Cell>> GRID) { grid = GRID; };
	// States //
	bool isStartSet() { return (gridProps->startPointSet); }
	bool isEndSet() { return (gridProps->endPointSet); }
	bool isStartAndEndSet() { return (gridProps->startPointSet && gridProps->endPointSet); }
	// Render Grid
	void RenderGrid();
	void RenderGridLines();
	void setGridColor(float r, float g, float b, float a = 1.0f);
	void setGridColor(glm::vec3 color);
private:
	std::shared_ptr<GridProps> gridProps;
	std::vector<std::vector<Cell>> grid;
	// Rendering
	GridRender gridRender;
	Renderer renderer;
	Quad fillQuad;
	glm::mat4 ViewProjectionMatrix;
};




