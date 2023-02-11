#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>


enum class cellState { VISITED = 0, UNVISITED, VISITING };
enum class cellType { NORMAL = 0, START, END, WALL, PATH };

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
	std::pair<uint32_t, uint32_t> coords;
	std::pair<uint32_t, uint32_t> parentCell; // Keep track of cell visited by for shortest path
	uint32_t weight;
	cellType m_Type;
	cellState m_State;
};
// Grid Properties //
struct GridProps {
	uint32_t width = 0;
	uint32_t height = 0;
	glm::vec4 gridColor = { 0.0f,0.0f,0.0f,1.0f };

	bool startPointSet = false;
	bool endPointSet = false;
	std::pair<uint32_t, uint32_t>startCoord;
	std::pair<uint32_t, uint32_t>endCoord;
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
	Grid(uint32_t row, uint32_t col);
	~Grid() = default;
	// Reset Functions
	void clearBoard();
	void clearPath();
	// Getters
	inline Cell getCell(uint32_t r, uint32_t c) { return grid[r][c]; }
	inline cellState getCellState(uint32_t r, uint32_t c) { return grid[r][c].m_State; }
	inline cellType getCellType(uint32_t r, uint32_t c) { return grid[r][c].m_Type; }
	inline std::pair<uint32_t, uint32_t> getCellParent(uint32_t r, uint32_t c) { return grid[r][c].parentCell; }
	inline uint32_t getHeight() { return gridProps->height; }
	inline uint32_t getWidth() { return gridProps->width; }
	// Setters
	void setCellState(uint32_t row, uint32_t col, cellState state);
	void setCellType(uint32_t row, uint32_t col, cellType type);
	void setCellParent(uint32_t r0, uint32_t c0, uint32_t r, uint32_t c);
	// Grid Getters //
	std::vector<std::vector<Cell>> getGrid() { return grid; }	
private:
	std::shared_ptr<GridProps> gridProps;
	std::vector<std::vector<Cell>> grid;
};




