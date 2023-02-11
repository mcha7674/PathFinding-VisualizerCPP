#pragma once
#include "../Grid/Grid.h"
#include <queue>
#include <chrono>
#include <thread>

namespace Algorithms
{
	// chrono settings
	using namespace std::this_thread;
	using namespace std::chrono_literals;
	using std::chrono::system_clock;

	enum class Type
	{
		None = 0,
		BFS, DFS,
		QuickSort, MergeSort,
		PostTraversal, PreTraversal
	};

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
		// Queue for bfs Algorithms
		std::queue <Cell> q;
		// directional arrays (delta row and col)
		int dr[4] = { 0, 1, 0, -1 };
		int dc[4] = { 1, 0, -1, 0 };
	};
}
