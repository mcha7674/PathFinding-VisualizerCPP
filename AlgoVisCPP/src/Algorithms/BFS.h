#pragma once
#include "PathFinder.h"

namespace Algorithms
{

	class BFS : public PathFinder
	{
	public:
		BFS(std::shared_ptr<Grid> grid, int numSearchDirections = 4)
			:m_Grid(grid)
		{
			Name = "BFS";
			m_numSearchDirections = numSearchDirections;
		}
		virtual ~BFS() = default;
		virtual void Init(std::pair<int, int> start) override;
		virtual bool Update() override;
		virtual bool PathUpdate() override;
	
	private:
		std::queue <int> q;
		std::shared_ptr<Grid> m_Grid;
		int pathLength =0;
	};



}
