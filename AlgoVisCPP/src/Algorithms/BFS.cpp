
#include "BFS.h"

namespace Algorithms
{
	// Init 
	void BFS::Init(std::pair<int, int> start)
	{
		// start clean
		Reset();
		m_Start = start;
		q.push(m_Grid->getGrid()[m_Start.first][m_Start.second]);
	}
	// Update 
	bool BFS::Update()
	{
		std::cout << "RUNNING BFS ALGO..." << std::endl;
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!q.empty() && !endFound) 
		{
			int r0 = q.front().coords.first;
			int c0 = q.front().coords.second;
			std::cout << "Parent of "<<r0<<","<<c0<<" is: (" << m_Grid->getCellParentCoord(r0, c0).first << ", " << m_Grid->getCellParentCoord(r0, c0).second << ")" << std::endl;
			q.pop();
			// iterate every neighbor
			for (int i = 0; i < 4; i++)
			{
				// First and Formost, draw the grid
				int r = r0 + dr[i];
				int c = c0 + dc[i];
				// boundary check
				if (r < 0 || c < 0 || r >= m_Grid->getHeight() || c >= m_Grid->getWidth()) { continue; }
				// skip wall cells
				if (m_Grid->getGrid()[r][c].m_Type == cellType::WALL) continue;
				// skip visited cells
				if (m_Grid->getGrid()[r][c].m_State == cellState::VISITED) continue;
				// set unvisited node as visited
				m_Grid->setCellState(r, c, cellState::VISITED);
				// save nodes parent
				m_Grid->setCellParent(r0, c0, r, c);
				// Check if this new cell is the end point
				if (m_Grid->getGrid()[r][c].m_Type == cellType::END)
				{	
					InitPath(r0, c0);
					endFound = true;
					break;
				}
				else { // just a normal node
					q.push(m_Grid->getGrid()[r][c]);
				}
			}
			return true; // Algorithm continues runnint
		}
		// Program only reaches this portion if End is found Or Queue was empty
		// is path update returns true, it means we continue to update next frame
		return PathUpdate();
	}

	bool BFS::PathUpdate(std::pair<int, int> &endCoordParent)
	{
		if (!endFound) return false;
		if (m_Grid->getCell(endCoordParent.first, endCoordParent.second).m_Type != cellType::START)
		{
			m_Grid->setCellType(endCoordParent.first, endCoordParent.second, cellType::PATH);
			endCoordParent.first = m_Grid->getCellParentCoord(endCoordParent.first, endCoordParent.second).first;
			endCoordParent.second = m_Grid->getCellParentCoord(endCoordParent.first, endCoordParent.second).second;
			return true;
		}	
		return false;
	}
	void BFS::InitPath(int r0, int c0)
	{
		// init hash with parent
		endCoordParent.first = m_Grid->getCellParentCoord(r, c).first;
		endCoordParent.second = m_Grid->getCellParentCoord(r, c).second;
		
	}

	void BFS::Reset()
	{
		q = {};
		endFound = false;
	}


}
