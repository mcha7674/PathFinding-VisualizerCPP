#pragma once
#include "PathFinder.h"

namespace Algorithms
{

	class DFS : public PathFinder
	{
	public:
		DFS(std::shared_ptr<Grid> grid, int numSearchDirections) 
			:m_Grid(grid)
		{
			Name = "DFS"; 
			m_numSearchDirections = numSearchDirections; 
		}
		virtual ~DFS() = default;
		virtual void Init(std::pair<int, int> start) override;
		virtual bool Update() override;
		virtual bool PathUpdate() override;
	private:
		std::stack <int> s;
		std::shared_ptr<Grid> m_Grid;
	};



}
