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
		virtual bool PathUpdate(std::pair<int, int> &endCoord) override;
		virtual void InitPath(int rEnd, int cEnd) override;
	private:
		std::queue <Cell> q;
		std::unordered_map<int, std::pair<int, int>> cellParentHash;
		std::shared_ptr<Grid> m_Grid;
	};



}
