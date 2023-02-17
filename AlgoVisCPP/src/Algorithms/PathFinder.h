#pragma once
#include "../Grid/Grid.h"
#include "../Grid/GridRender.h"
#include <queue>
#include <stack>
#include <chrono>
#include <thread>

namespace Algorithms
{
	// chrono settings
	using namespace std::this_thread;
	using namespace std::chrono_literals;
	using std::chrono::system_clock;

	class PathFinder 
	{
	public:
		virtual ~PathFinder() = default;
		virtual bool Update() = 0;
		virtual bool PathUpdate() = 0;
		// Initializers
		virtual inline void Init(std::pair<int, int> start) { 
			m_Start = start; endFound = false; parentHash.clear(); 
		}
		virtual inline void InitPath(int parentCell) { currCell = parentCell; }
		// Getters
		virtual inline std::string getName() { return Name; }
		virtual inline bool getEndState() { return endFound; }
		// Setters
		virtual inline void setNumSearchDirections(int num) {m_numSearchDirections = num; }
	protected:
		// Every Algo Has A Name
		std::string Name;
		// Every Algorithm Is given a starting Cell
		std::pair<int, int> m_Start;
		// Final Path Attributes
		bool endFound;
		// Temp Storage for final path
		int currCell; 
		// A Hash table that Stores the Cell ID With its Parent Cell's Coordinate
		std::unordered_map<int, std::pair<int, int>> parentHash;
		int m_numSearchDirections;
		static constexpr const int dr4[4] = { 0, 1, 0, -1 };
		static constexpr const int dc4[4] = { 1, 0, -1, 0 };
		static constexpr const int dr8[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		static constexpr const int dc8[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	};
}
