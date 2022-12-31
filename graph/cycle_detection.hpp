#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
vi cycle_detection(Graph<>& G) {
	int N = SZ(G);
	vi cycle;
	vb visited(N);
	vb seen(N);
	auto dfs = [&](auto f, int cur, int pre = -1) -> int {
		if(seen[cur]) {
			return cur;
		}
		seen[cur] = true;
		for(Edge e: G[cur]) {
			if(e.to == pre) continue;
			if(visited[e.to]) continue;
			int begin = f(f, e.to, cur);
			if(begin == -2) {
				visited[cur] = true;
				return -2;
			}
			if(begin == -1) continue;
			if(begin == cur) {
				cycle.push_back(cur);
				visited[cur] = true;
				return -2;
			}
			cycle.push_back(cur);
			visited[cur] = true;
			return begin;
		}
		visited[cur] = true;
		return -1;
	};
	REP(i, N) {
		if(!cycle.empty()) break;
		if(visited[i]) continue;
		dfs(dfs, i);
	}
	if(cycle.empty()) {
		return vi();
	}
	rev(cycle);
	vi C(N, -1);
	REP(i, SZ(cycle)) {
		C[cycle[i]] = i;
	}
	int ind = 0;
	vi ret;
	using P = pair<int, int>;
	set<P> S;
	while(ind != -1) {
		int v = cycle[ind];
		ret.push_back(v);
		S.insert({ ind, SZ(ret) - 1 });
		int next = -1;
		int pre = -1;
		for(Edge e: G[v]) {
			int n = C[e.to];
			if(n == -1) continue;
			if(n < ind) {
				auto [a, b] = *S.lower_bound({ n, 0 });
				if(a != n) continue;
				chmax(pre, b);
			}
			chmax(next, n);
		}
		if(pre != -1) {
			vi new_ret;
			FOR(i, pre, SZ(ret)) {
				new_ret.push_back(ret[i]);
			}
			ret = new_ret;
			return ret;
		}
		ind = next;
	}
	return ret;
}