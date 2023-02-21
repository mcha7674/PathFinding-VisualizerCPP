#include "Dijkstra.h"

namespace Algorithms
{
	// Init 
	void Dijkstra::Init(std::pair<int, int> start)
	{
		// start clean
		PathFinder::Init(start);
		// costs hash
		cost.clear();
		// Clear Previous Queue if Any
		minQ = {};
		// push Cell ID
		minQ.push({ m_Grid->getCellID(start.first, start.second), 0 });
		// Init Hash with initial distance of zero for the start node
		cost[minQ.top().first] = 0;
	}
	// Update 
	bool Dijkstra::Update()
	{
		std::cout << "RUNNING Dijkstra ALGO..." << std::endl;
		std::cout << "Search Type: " << m_numSearchDirections << std::endl;
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!minQ.empty() && !endFound)
		{
			int cell = minQ.top().first;
			int cellCost = minQ.top().second;
			int r0 = m_Grid->getCellCoord(cell).first;
			int c0 = m_Grid->getCellCoord(cell).second;
			minQ.pop();
			m_Grid->setCellState(r0, c0, cellState::VISITED);
			// iterate every neighbor
			for (int i = 0; i < m_numSearchDirections; i++)
			{
				int r = r0 + dr4[i];
				int c = c0 + dc4[i];
				if (m_numSearchDirections == 8) {
					r = r0 + dr8[i];
					c = c0 + dc8[i];
				}
				// boundary check
				if (r < 0 || c < 0 || r >= m_Grid->getHeight() || c >= m_Grid->getWidth()) { continue; }
				// skip wall cells
				if (m_Grid->getCellType(r, c) == cellType::WALL) continue;
				// skip visited cells
				if (m_Grid->getCellState(r, c) == cellState::VISITED || m_Grid->getCellState(r, c) == cellState::VISITING) continue;
				// set unvisited node as visiting
				m_Grid->setCellState(r, c, cellState::VISITING);
				// save nodes parent
				parentHash[m_Grid->getCellID(r, c)] = { r0, c0 };
				// update visiting node cost
				int prevCost = cellCost;
				int currCellWeight = m_Grid->getCellWeight(r, c);
				int newCost = prevCost + currCellWeight;
				cost[m_Grid->getCellID(r, c)] = newCost;
				// Check if this new cell is the end point
				if (m_Grid->getCellType(r, c) == cellType::END)
				{
					InitPath(cell);
					endFound = true;
					break;
				}
				else { minQ.push( { m_Grid->getCellID(r, c), newCost } ); }
			}
			return true; // Algorithm continues runnint
		}
		// Program only reaches this portion if End is found Or Queue was empty
		// is path update returns true, it means we continue to update next frame
		return PathUpdate();
	}

	bool Dijkstra::PathUpdate()
	{
		if (!endFound) return false;
		if (m_Grid->getCellType(currCell) != cellType::START)
		{
			m_Grid->setCellType(m_Grid->getCellCoord(currCell).first, m_Grid->getCellCoord(currCell).second, cellType::PATH);
			currCell = m_Grid->getCellID(parentHash[currCell].first, parentHash[currCell].second);
			//sleep_for(0.03s);
			return true;
		}
		return false;
	}

}
