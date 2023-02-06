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
	cellState m_State;
	Cell() {
		weight = 0; 
		m_State = UNVISITED;
	}
};


class Grid
{
public:
	Grid(Renderer& renderer, glm::mat4 ViewProjectionMatrix);
	// init Grid - fill grid with defaults with correct size.
	void Init(int row, int col);
	// Draw Grid - using passed in params, render the grid
	void DrawGrid();
	// Getters //
	std::vector<std::vector<Cell>> getGrid() { return grid; }

private:
	// Grid Properties //
	std::vector<std::vector<Cell>> grid;
	int width;
	int height;
	// Visit Fills
	Quad visitedFill;
	Quad visitingFill;

	// Rendering Properties //
	Renderer renderer;
	glm::mat4 ViewProjectionMatrix;
	Shader* cell_shader;
	Transform trans;
	VertexArray va; // (GenVertexArray)
	VertexBuffer* vb; // (Gen, Bind, Data Implement)
	VertexBufferLayout layout;


	
	
};

