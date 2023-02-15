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
		BFS, DFS, AStar, Dijkstra
	};

	class PathFinder 
	{
	public:
		virtual ~PathFinder() = default;
		//virtual void Execute(std::pair<int, int> start) = 0;
		virtual void Init(std::pair<int, int> start) = 0;
		virtual bool Update() = 0;
		virtual void Reset() = 0;
		virtual bool PathUpdate() = 0;
		virtual void InitPath(int parentCell) = 0;
		// Getters
		virtual bool getEndState() { return endFound; }
	protected:
		// Every Algorithm Is given a starting Cell
		std::pair<int, int> m_Start;
		// Final Path Attributes
		bool endFound;
		int currCell; // Temp Storage for final path
		std::unordered_map<int, std::pair<int, int>> parentHash;
		// directional arrays (delta row and col)
		static constexpr const int dr[4] = { 0, 1, 0, -1 };
		static constexpr const int dc[4] = { 1, 0, -1, 0 };
		/*static constexpr const int dr[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		static constexpr const int dc[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };*/
	};
}
