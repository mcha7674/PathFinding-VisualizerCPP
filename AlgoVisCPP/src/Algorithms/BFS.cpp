
#include "BFS.h"

namespace Algorithms
{
	// Init 
	void BFS::Init(std::pair<int, int> start)
	{
		// start clean
		PathFinder::Init(start);
		// Clear Previous Queue if Any
		q = {};
		// push Cell ID
		q.push(m_Grid->GetCellID(m_Start.first, m_Start.second));
	}
	// Update 
	bool BFS::Update()
	{
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!q.empty() && !endFound) 
		{
			int cell = q.front();
			int r0 = m_Grid->GetCellCoord(cell).first;
			int c0 = m_Grid->GetCellCoord(cell).second;
			q.pop();
			m_Grid->SetCellState(r0, c0, cellState::VISITED);
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
				if (r < 0 || c < 0 || r >= m_Grid->GetHeight() || c >= m_Grid->GetWidth()) { continue; }
				// skip wall cells
				if (m_Grid->GetCellType(r, c) == cellType::WALL) continue;
				// skip visited cells
				if (m_Grid->GetCellState(r, c) == cellState::VISITED || m_Grid->GetCellState(r, c) == cellState::VISITING) continue;
				// set unvisited node as visiting
				m_Grid->SetCellState(r, c, cellState::VISITING);
				// save nodes parent
				parentHash[m_Grid->GetCellID(r, c)] = { r0, c0 };
				// Check if this new cell is the end point
				if (m_Grid->GetCellType(r, c) == cellType::END)
				{	
					InitPath(cell);
					endFound = true;
					break;
				}
				q.push(m_Grid->GetCellID(r, c));
			}
			return true; // Algorithm continues runnint
		}
		// Program only reaches this portion if End is found Or Queue was empty
		// is path update returns true, it means we continue to update next frame
		return PathUpdate();
	}

	bool BFS::PathUpdate()
	{
		if (!endFound) return false;
		if (m_Grid->GetCellType(currCell) != cellType::START)
		{
			m_Grid->SetCellType(m_Grid->GetCellCoord(currCell).first, m_Grid->GetCellCoord(currCell).second, cellType::PATH);
			currCell = m_Grid->GetCellID(parentHash[currCell].first, parentHash[currCell].second);
			return true;
		}	
		return false;
	}

}
