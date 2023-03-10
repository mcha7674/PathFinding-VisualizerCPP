#pragma once

#include "PathFinder.h"
namespace Algorithms
{

	
	// compare the addition of both the Fcost and the Hcost
	// if both fcosts are the same, but 1 has a lower h cost, their sum will be lower and the priority queue will swap them.
	
	class AstarCompare
	{
	public:
		bool operator()(std::pair<int, std::vector<int>> pair1, std::pair<int, std::vector<int>> pair2)
		{
			if ((pair1.second[0]) == (pair2.second[0])) {
				if ((pair1.second[1]) == (pair2.second[1]))
					return (pair1.second[2]) > (pair2.second[2]);
				else return (pair1.second[1]) > (pair2.second[1]);
			}
			return (pair1.second[0]) > (pair2.second[0]);
		}
	};

	class Astar : public PathFinder
	{
	public:
		Astar(std::shared_ptr<Grid> grid, int numSearchDirections = 4)
			:m_Grid(grid)
		{
			Name = "Astar";
			m_numSearchDirections = numSearchDirections;	
			currHeuristic == Heuristic::MANHATTAN;

		}

		virtual ~Astar() = default;
		virtual void Init(std::pair<int, int> start) override;
		virtual bool Update() override;
		virtual bool PathUpdate() override;
		int EuclidHeuristic(int r, int c);
		int ChebyshevHeuristic(int r, int c);
		int ManhattanHeuristic(int r, int c);
		
	private:
		// minimum priority queue - first element in pair is the Cell ID, the second element is {FCost, HCost}
		//std::priority_queue <std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, AstarCompare > minQ;
		std::priority_queue <std::pair<int, std::vector<int>>, std::vector<std::pair<int, std::vector<int>>>, AstarCompare > minQ;
		std::shared_ptr<Grid> m_Grid;
		std::unordered_map<int, int> Gcost; // cell id, cost
		std::pair<int, int> startCoord;
		std::pair<int, int> endCoord;
		int count;
		
	};

}