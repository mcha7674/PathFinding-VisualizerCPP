#pragma once
#include <iostream>
#include <glm/glm.hpp>

enum class cellState { VISITED = 0, UNVISITED, VISITING };
enum class cellType { NORMAL = 0, START, END, WALL, PATH };

struct GridProperties
{
	int width = 0; // In Cell Units
	int height = 0;
	bool startPointSet = false;
	bool endPointSet = false;
	std::pair<int, int>startCoord;
	std::pair<int, int>endCoord;
	GridProperties() { SetDefault(); }
	void SetDefault() {
		startPointSet = false;
		endPointSet = false;
		startCoord = { -1, -1 };
		endCoord = { -2, -2 };
	}
};

struct Cell
{
	Cell(int r, int c)
		:row(r), col(c)
	{ 
		SetDefault(); 
	}
	void SetDefault()
	{
		id = -1;
		weight = 0;
		m_State = cellState::UNVISITED;
		m_Type = cellType::NORMAL;
	}
	int row;
	int col;
	int weight;
	cellType m_Type;
	cellState m_State;
	int id;
};
