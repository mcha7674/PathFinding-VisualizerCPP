#include "DFS.h"


namespace Algorithms
{
	// Init 
	void DFS::Init(std::pair<int, int> start)
	{
		PathFinder::Init(start);
		// Clear Previous Stack
		s = {};
		// Push Starting Cell ID
		s.push(m_Grid->getCellID(m_Start.first, m_Start.second));
	} 
	bool DFS::Update()
	{
		std::cout << "RUNNING DFS ALGO..." << std::endl;
		std::cout << "Search Type: " << m_numSearchDirections << std::endl;
		// if s is NOT empty AND end has Not be found yet, keep searching!
		if (!s.empty() && !endFound)
		{
			// current
			int cell = s.top();
			int r0 = m_Grid->getCellCoord(cell).first;
			int c0 = m_Grid->getCellCoord(cell).second;
			s.pop();
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
				// Check if this new cell is the end point
				if (m_Grid->getCellType(r, c) == cellType::END)
				{
					InitPath(cell);
					endFound = true;
					break;
				}
				else { // just a normal node
					s.push(m_Grid->getCellID(r, c));
				}
			}
			return true; // Algorithm continues runnint
		}
		return PathUpdate();
	}

	bool DFS::PathUpdate()
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
