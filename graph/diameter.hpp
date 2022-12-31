#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
namespace inner_diameter {
using P = pair<int, int>;
P dfs(int cur, int pre, Graph<> &G) {
	P p({ 0, cur });
	for(Edge e: G[cur]) {
		if(e.to == pre) continue;
		auto [dis, v] = dfs(e.to, cur, G);
		chmax(p, P(dis + 1, v));
	}
	return p;
}
}

int get_diameter(Graph<> &G) {
	auto [dis, v] = inner_diameter::dfs(0, -1, G);
	auto [dis2, v2] = inner_diameter::dfs(v, -1, G);
	return dis2;
}