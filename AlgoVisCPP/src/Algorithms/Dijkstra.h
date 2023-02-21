#pragma once

#include "PathFinder.h"

namespace Algorithms
{

	class PairCompare
	{
	public:
		bool operator()(std::pair<int, int> pair1, std::pair<int, int> pair2)
		{
			return (pair1.second > pair2.second);
		}
	};

	class Dijkstra : public PathFinder
	{
	public:
		Dijkstra(std::shared_ptr<Grid> grid, int numSearchDirections = 4)
			:m_Grid(grid)
		{
			Name = "Dijkstra";
			m_numSearchDirections = numSearchDirections;
			// Initiate Node Costs:
			for (int cellID = 0; cellID < (m_Grid->getHeight()*m_Grid->getWidth()); cellID++) {
				cost[cellID] = INT_MAX;
			}
		}
		virtual ~Dijkstra() = default;
		virtual void Init(std::pair<int, int> start) override;
		virtual bool Update() override;
		virtual bool PathUpdate() override;
	private:
		// cell queue
		std::queue <int> q;
		// minimum priority queue - first element in pair is the Cell ID, the second is its Cost
		std::priority_queue <std::pair<int, int>, std::vector<std::pair<int, int>>, PairCompare > minQ;
		std::shared_ptr<Grid> m_Grid;
		std::unordered_map<int, int> cost; // cell id, cost
	};

}