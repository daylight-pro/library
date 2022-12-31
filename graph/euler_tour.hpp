#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
/// @brief オイラーツアーを求めます
/// @param G グラフ
/// @param root 木の根
/// @return 各頂点に割り当てられた閉区間
vector<pair<int, int>> eulerTour(Graph<> &G, int root = 0) {
	int N = SZ(G);
	vector<pair<int, int>> ret(N);
	int turn = 0;
	auto dfs = [&](auto f, int cur, int pre) -> void {
		ret[cur].first = turn++;
		for(Edge e: G[cur]) {
			if(e.to == pre) continue;
			f(f, e.to, cur);
		}
		ret[cur].second = turn++;
	};
	dfs(dfs, root, -1);
	return ret;
}