#include "Dijkstra.h"

namespace Algorithms
{
	// Init 
	void Dijkstra::Init(std::pair<int, int> start)
	{
		// start clean
		PathFinder::Init(start);
		// clear previous hash
		cost.clear();
		// fill hash with default cost values for every cell
		for (int id = 0; id < m_Grid->getHeight() * m_Grid->getWidth(); id++) {
			cost[id] = INT_MAX;
		}
		// Clear Previous Queue if Any
		minQ = {};
		// push Cell ID
		minQ.push({ m_Grid->getCellID(start.first, start.second), 0 });
		// Init cost hash with start 
		cost[minQ.top().first] = 0;
	}
	// Update 
	bool Dijkstra::Update()
	{
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!minQ.empty() && !endFound)
		{
			// grab cell id
			int curr = minQ.top().first;
			int currCost = cost[curr] + 1; // add 1 because each move to another cell by default costs 1
			int r0 = m_Grid->getCellCoord(curr).first;
			int c0 = m_Grid->getCellCoord(curr).second;
			minQ.pop();
			// This cell is not a visited cell
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
				// End Condition
				if (m_Grid->getCellType(r, c) == cellType::END)
				{
					InitPath(curr);
					endFound = true;
					break;
				} //
				else if (currCost < cost[m_Grid->getCellID(r, c)]) {
					// mark this cell as a visiting cell!
					m_Grid->setCellState(r, c, cellState::VISITING);
					// update gscore of this neighbor cell, apply its weight if it has any
					cost[m_Grid->getCellID(r, c)] = currCost + m_Grid->getCellWeight(r, c);
					int neighborCost = cost[m_Grid->getCellID(r, c)];
					// Update the Parent of the neighbor to the current cell
					parentHash[m_Grid->getCellID(r, c)] = { r0, c0 };
					// Add the neighbor to the priority queue
					minQ.push({ m_Grid->getCellID(r, c), neighborCost });
				}
			}
			return true; // Algorithm continues running
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
			return true;
		}
		return false;
	}

}
