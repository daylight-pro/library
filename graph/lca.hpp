#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
struct LCA {
private:
	vvi parent;
	vi dis;
	void dfs(int cur, int pre, const Graph<>& G, int d) {
		parent[0][cur] = pre;
		dis[cur] = d;
		for(auto e: G[cur]) {
			if(e.to == pre) continue;
			dfs(e.to, cur, G, d + e.cost);
		}
	}

public:
	LCA(const Graph<>& G) {
		int N = SZ(G);
		int K = 1;
		while((1 << K) < N) K++;
		parent = vvi(K, vi(N, -1));
		dis = vi(N, -1);
		dfs(0, -1, G, 0);
		REP(i, K - 1) {
			REP(j, N) {
				if(parent[i][j] < 0) {
					parent[i + 1][j] = -1;
				} else {
					parent[i + 1][j]
						= parent[i][parent[i][j]];
				}
			}
		}
	}
	int query(int u, int v, int root) {
		return query(u, v) ^ query(v, root)
			^ query(u, root);
	}
	int query(int u, int v) {
		assert(u >= 0 && v >= 0 && u < SZ(dis)
			   && v < SZ(dis) && "invalid vertex index");
		if(dis[u] < dis[v]) swap(u, v);
		int K = SZ(parent);
		REP(i, K) {
			if((dis[u] - dis[v]) >> i & 1) {
				u = parent[i][u];
			}
		}
		if(u == v) return u;
		REPR(i, K) {
			if(parent[i][u] != parent[i][v]) {
				u = parent[i][u];
				v = parent[i][v];
			}
		}
		return parent[0][u];
	}

	int get_dis(int u, int v) {
		assert(u >= 0 && v >= 0 && u < SZ(dis)
			   && v < SZ(dis) && "invalid vertex index");
		return dis[u] + dis[v] - 2 * dis[query(u, v)];
	}
};