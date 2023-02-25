#include "Astar.h"

namespace Algorithms
{

	int Astar::EuclidHeuristic(int r, int c)
	{
		int endR = endCoord.first;
		int endC = endCoord.second;
		// Euclidean distance Heuristic
		return (int)(sqrt(pow((endR - r),2) + pow((endC - c),2)));
	}
	int Astar::ChebyshevHeuristic(int r, int c)
	{
		int endR = endCoord.first;
		int endC = endCoord.second;
		int dx = abs(endR - r);
		int dy = abs(endC - c);
		return std::max(dx, dy);
	}
	int Astar::ManhattanHeuristic(int r, int c)
	{
		int endR = endCoord.first;
		int endC = endCoord.second;
		// Euclidean distance Heuristic
		return abs(endR - r) + abs(endC - c);
	}
	
	// Init 
	void Astar::Init(std::pair<int, int> start)
	{
		// start clean
		PathFinder::Init(start);
		// costs hash
		Gcost.clear();
		count = 0;
		// fill cost hash with default gcost values for every cell
		for (int id = 0; id < m_Grid->getHeight() * m_Grid->getWidth(); id++) {
			Gcost[id] = INT_MAX;
		}
		// Clear Previous Queue if Any
		minQ = {};
		// push Cell ID
		minQ.push({ m_Grid->getCellID(start.first, start.second), {0, 0, 0} });
		// Init Hash with initial distance of zero for the start node
		Gcost[minQ.top().first] = 0;
		// Initialize Coords
		startCoord = m_Grid->getStartCoord();
		endCoord = m_Grid->getEndCoord();
	}
	
	// Update 
	bool Astar::Update()
	{
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!minQ.empty() && !endFound)
		{
			// grab cell id
			int curr = minQ.top().first;
			int r0 = m_Grid->getCellCoord(curr).first;
			int c0 = m_Grid->getCellCoord(curr).second;
			int currGscore = Gcost[curr] + 1; // add 1 because each move to another cell by default costs 1
			if (m_Grid->getCellCoord(curr) == startCoord) { currGscore = Gcost[curr];}
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
				if (m_Grid->getCellState(r, c) == cellState::VISITED || m_Grid->getCellState(r, c) == cellState::VISITING) continue;
				// End Condition
				if (m_Grid->getCellType(r, c) == cellType::END)
				{
					InitPath(curr);
					endFound = true;
					break;
				} 
				if (currGscore < Gcost[m_Grid->getCellID(r, c)]) {
					count++;
					// mark this cell as a visiting cell!
					m_Grid->setCellState(r, c, cellState::VISITING);
					// update gscore of this neighbor cell, apply its weight if it has any
					Gcost[m_Grid->getCellID(r, c)] = currGscore + m_Grid->getCellWeight(r, c);
					int GCost = Gcost[m_Grid->getCellID(r, c)];
					// calculate the heuristic
					int HCost = 0;
					if (m_numSearchDirections == 4) { HCost = ChebyshevHeuristic(r, c); }
					else { HCost = ChebyshevHeuristic(r, c); }
					// Calculate the FCost
					int FCost = GCost + HCost;
					// Update the Parent of the neighbor to the current cell
					parentHash[m_Grid->getCellID(r, c)] = { r0, c0 };
					// Add the neighbor to the priority queue
					minQ.push({ m_Grid->getCellID(r, c), {FCost, HCost, count} });
				}
			}
			return true; // Algorithm continues running
		}
		// Program only reaches this portion if End is found Or Queue was empty
		// is path update returns true, it means we continue to update next frame
		return PathUpdate();
	}

	bool Astar::PathUpdate()
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
