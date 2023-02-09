#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>
#include "Base_Models/Quad.h"

enum class cellState{VISITED = 0, UNVISITED, VISITING};
enum class cellType{NORMAL = 0, START, END, WALL, PATH};

struct Cell
{
	std::pair<int, int> coords;
	std::pair<int, int> parentCell; // Keep track of cell visited by for shortest path
	int weight;
	cellType m_Type;
	cellState m_State;
	// Cell Methods
	Cell() { reset(); }
	void reset()
	{
		weight = 0;
		m_State = cellState::UNVISITED;
		m_Type = cellType::NORMAL;
	}
};

// Grid Properties //
struct GridProperties {
	int width = 0;
	int height = 0;
	glm::vec4 gridColor = { 0.0f,0.0f,0.0f,1.0f };

	bool startPointSet;
	bool endPointSet;
	std::pair<int, int>startCoord;
	std::pair<int, int>endCoord;
	GridProperties() { reset(); }
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
	Grid(Renderer& renderer, glm::mat4 ViewProjectionMatrix);
	~Grid() { delete grid_shader; grid.clear(); };
	// init Grid - fill grid with defaults with correct size.
	void Init(int row, int col);
	// Draw Grid - using passed in params, render the grid
	void DrawGrid();
	void DrawGridLines();
	void resetGrid();
	// Cell Setter Functions
	void setCellState(int row, int col, cellState state);
	void setCellType(int row, int col, cellType type);
	void setCellParent(int r0, int c0, int r, int c);
	// Cell Getters
	inline Cell getCell(int r, int c) { return grid[r][c]; }
	inline std::pair<int,int> getCellParent(int r, int c) { return grid[r][c].parentCell; }
	// Grid Getters //
	std::vector<std::vector<Cell>> getGrid() { return grid; }
	std::shared_ptr<GridProperties> getGridProps() { return gridProps; }
	// Grid Setters //
	void setGridColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void setGridColor(glm::vec4 rgba = { 1.0f,1.0f,1.0f,1.0f });
private:
	std::shared_ptr<GridProperties> gridProps;
	std::vector<std::vector<Cell>> grid;
	// A cell Is filled with a color
	Quad fillQuad;
	// Rendering Properties //
	Renderer renderer;
	glm::mat4 ViewProjectionMatrix;
	Shader *grid_shader;
	VertexArray va; // (GenVertexArray)
	std::unique_ptr<VertexBuffer> vb; // (Gen, Bind, Data Implement)
	VertexBufferLayout layout;
};

