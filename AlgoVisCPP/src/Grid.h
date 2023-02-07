#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>
#include "Base_Models/Quad.h"

enum cellState{VISITED, UNVISITED, VISITING};

struct Cell
{
	int weight;
	bool isStart;
	bool isEnd;
	cellState m_State;
	Cell() {
		weight = 0; 
		m_State = UNVISITED;
		isStart = false;
		isEnd = false;
	}
};

// Grid Properties //
struct GridProperties {
	int width = 0;
	int height = 0;
	bool startPointSet = false;
	bool endPointSet = false;
	glm::vec4 gridColor = { 0.0f,0.0f,0.0f,1.0f };
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
	void resetGrid();
	// Function to fill a cell with the correct color if visiting and update its state.
	void visitCell(int x, int y, bool isStart = false, bool isEnd = false);

	// Getters //
	std::vector<std::vector<Cell>> getGrid() { return grid; }
	std::shared_ptr<GridProperties> getGridProps() { return gridProps; }
	// Setters //
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

