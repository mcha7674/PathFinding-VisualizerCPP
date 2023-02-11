
#include "BFS.h"

namespace Algorithms
{
	void BFS::Execute(std::pair<int, int> start)
	{
		std::cout << "RUNNING BFS ALGO..." << std::endl;
		// Start with a clean bfs
		Reset();
		int rowHeight = m_Grid->getHeight();
		int colWidth = m_Grid->getWidth();
		// Add start cell to queue
		q.push(m_Grid->getGrid()[start.first][start.second]);
		while (!q.empty())
		{
			int r0 = q.front().coords.first;
			int c0 = q.front().coords.second;
			q.pop();
			// iterate every neighbor
			for (int i = 0; i < 4; i++)
			{
				// First and Formost, draw the grid
				int r = r0 + dr[i];
				int c = c0 + dc[i];
				// boundary check
				if (r < 0 || c < 0 || r >= rowHeight || c >= colWidth) { continue; }
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
				{	// while 
					int prev_r = r0;
					int prev_c = c0;
					while (m_Grid->getCell(prev_r, prev_c).m_Type != cellType::START)
					{
						m_Grid->setCellType(prev_r, prev_c, cellType::PATH);
						prev_r = m_Grid->getCellParent(prev_r, prev_c).first;
						prev_c = m_Grid->getCellParent(prev_r, prev_c).second;
					}
					gridStates.push_back(m_Grid->getGrid());
					return;
				}
				else { // just a normal node
					q.push(m_Grid->getGrid()[r][c]);
				}
			}
			// add grid states
			gridStates.push_back(m_Grid->getGrid());
		}
		return; // return true as algorithm is completed
	}

	void BFS::Reset()
	{
		q = {};
		gridStates = {};
	}


}
