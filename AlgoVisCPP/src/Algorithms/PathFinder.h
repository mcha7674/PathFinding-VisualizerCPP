#pragma once
#include "Algorithms.h"
#include "../Grid.h"
#include <queue>

namespace Algorithms
{
	class PathFinder 
	{
	public:
		/*PathFinder(std::shared_ptr<Grid> grid) 
			:m_Grid(grid)
		{
		}*/
		PathFinder();
		bool bfs(std::pair<int,int>startCoords, std::shared_ptr<Grid> m_Grid);
		void reset();
	private:
		// Queue for the bfs Algorithm
		std::queue <Cell> q;
		// Pointer to the main grid
		//std::shared_ptr<Grid> m_Grid;
		// directional arrays (delta row and col)
		int dr[4] = { 0, 1, 0, -1 };
		int dc[4] = { 1, 0, -1, 0 };
	};
}
