
#include "BFS.h"

namespace Algorithms
{
	// Init 
	void BFS::Init(std::pair<int, int> start)
	{
		// start clean
		Reset();
		m_Start = start;
		// push Cell ID
		q.push(m_Grid->getCellID(m_Start.first, m_Start.second));
	}
	// Update 
	bool BFS::Update()
	{
		std::cout << "RUNNING BFS ALGO..." << std::endl;
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!q.empty() && !endFound) 
		{
			int r0 = m_Grid->getCellCoord(q.front()).first;
			int c0 = m_Grid->getCellCoord(q.front()).second;
			q.pop();
			//m_Grid->setCellState(r0, c0, cellState::VISITED);
			// iterate every neighbor
			for (int i = 0; i < 4; i++)
			{
				// First and Formost, draw the grid
				int r = r0 + dr[i];
				int c = c0 + dc[i];
				// boundary check
				if (r < 0 || c < 0 || r >= m_Grid->getHeight() || c >= m_Grid->getWidth()) { continue; }
				// skip wall cells
				if (m_Grid->getCellType(r, c) == cellType::WALL) continue;
				// skip visited cells
				if (m_Grid->getCellState(r, c) == cellState::VISITED) continue;
				// set unvisited node as visited
				m_Grid->setCellState(r, c, cellState::VISITED);
				// save nodes parent
				cellParentHash[m_Grid->getCellID(r, c)] = { r0, c0 };
				// Check if this new cell is the end point
				if (m_Grid->getCellType(r, c) == cellType::END)
				{	
					InitPath(r0, c0, r, c);
					endFound = true;
					break;
				}
				else { // just a normal node
					q.push(m_Grid->getCellID(r, c));
				}
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
		if (m_Grid->getCellType(currCell) != cellType::START)
		{
			m_Grid->setCellType(m_Grid->getCellCoord(currCell).first, m_Grid->getCellCoord(currCell).second, cellType::PATH);
			currCell = m_Grid->getCellID(cellParentHash[currCell].first, cellParentHash[currCell].second);
			return true;
		}	
		return false;
	}
	void BFS::InitPath(int r0, int c0, int r, int c)
	{
		// init hash with parent
		//cellParentHash[m_Grid->getCellID(r, c)] = {r0, c0};
		currCell = m_Grid->getCellID(r0, c0);
	}

	void BFS::Reset()
	{
		q = {};
		endFound = false;
		cellParentHash.clear();
	}


}
