#pragma once
#include "PathFinder.h"

namespace Algorithms
{

	class BFS : public PathFinder
	{
	public:
		BFS(std::shared_ptr<Grid> grid) :m_Grid(grid) {}
		virtual ~BFS() = default;
		virtual void Init(std::pair<int, int> start) override;
		virtual bool Update() override;
		virtual void Reset() override;
		virtual bool PathUpdate() override;
		virtual void InitPath(int r0, int c0, int rEnd, int cEnd) override;
	private:
		std::queue <int> q;
		std::unordered_map<int, std::pair<int, int>> cellParentHash;
		int currCell;
		std::shared_ptr<Grid> m_Grid;
	};



}
