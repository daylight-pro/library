#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
/// @brief 単一始点最短経路長問題を解く
/// @param s 始点
/// @param G グラフ
/// @return ret[i]:sからiへの最短経路長
vll dijkstra(int s, Graph<ll>& G) {
	using P = pair<ll, int>;
	priority_queue<P, vector<P>, greater<>> que;
	vll D(SZ(G), LINF);
	D[s] = 0;
	que.push(P(0, s));
	while(!que.empty()) {
		P p = que.top();
		que.pop();
		int v = p.second;
		if(D[v] < p.first) continue;
		for(Edge e: G[v]) {
			if(D[e.to] > D[v] + e.cost) {
				D[e.to] = D[v] + e.cost;
				que.push(P(D[e.to], e.to));
			}
		}
	}
	return D;
}