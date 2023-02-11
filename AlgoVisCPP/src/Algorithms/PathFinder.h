#pragma once
#include "../Grid/Grid.h"
#include "../Grid/GridRender.h"
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
		BFS, DFS, Dijkstra,
		AStar
	};

	class PathFinder 
	{
	public:
		virtual ~PathFinder() = default;
		virtual void Execute(std::pair<int, int> start) = 0;
		virtual void Reset() = 0;
		virtual std::vector<std::vector<std::vector<Cell>>> getGridStates()
		{
			return gridStates;
		}
		virtual int getGridStateSize()
		{
			return (int)gridStates.size();
		}
	protected:
		std::vector<std::vector<std::vector<Cell>>> gridStates;
		// directional arrays (delta row and col)
		static constexpr const int dr[4] = { 0, 1, 0, -1 };
		static constexpr const int dc[4] = { 1, 0, -1, 0 };
	};
}
