#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
struct LCA {
private:
	vvi parent;
	vi dis;
	vi simple_dis;
	void dfs(int cur, int pre, const Graph<>& G, int d,
			 int sd) {
		parent[0][cur] = pre;
		dis[cur] = d;
		simple_dis[cur] = sd;
		for(auto e: G[cur]) {
			if(e.to == pre) continue;
			dfs(e.to, cur, G, d + e.cost, sd + 1);
		}
	}

public:
	LCA(const Graph<>& G) {
		int N = SZ(G);
		int K = 1;
		while((1 << K) < N) K++;
		parent = vvi(K, vi(N, -1));
		dis = vi(N, -1);
		simple_dis = vi(N, -1);
		dfs(0, -1, G, 0, 0);
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
	/// @brief uとvの根をrootとした時のLCAを求める
	/// @param u クエリの頂点1
	/// @param v クエリの頂点2
	/// @param root クエリの根
	/// @return LCAの頂点番号
	int query(int u, int v, int root) {
		return query(u, v) ^ query(v, root)
			^ query(u, root);
	}
	/// @brief uとvの根を0とした時のLCAを求める
	/// @param u クエリの頂点1
	/// @param v クエリの頂点2
	/// @return LCAの頂点番号
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

	/// @brief 頂点fromから頂点toに向かってcnt回たどった位置にある頂点
	/// @param from 始点
	/// @param to 終点
	/// @param cnt たどる回数
	/// @return 頂点番号(存在しない場合は-1)
	int jump(int from, int to, int cnt) {
		if(cnt < 0 || get_simple_dis(from, to) < cnt) {
			return -1;
		}
		int l = query(from, to);
		if(cnt <= get_simple_dis(from, l)) {
			int cur = from;
			int K = SZ(parent);
			REP(i, K) {
				if(cnt >> i & 1) {
					cur = parent[i][cur];
				}
			}
			return cur;
		}
		cnt = get_simple_dis(from, to) - cnt;
		int cur = to;
		int K = SZ(parent);
		REP(i, K) {
			if(cnt >> i & 1) {
				cur = parent[i][cur];
			}
		}
		return cur;
	}

	/// @brief uとvの距離を求める
	/// @param u クエリの頂点1
	/// @param v クエリの頂点2
	/// @return 距離
	int get_dis(int u, int v) {
		assert(u >= 0 && v >= 0 && u < SZ(dis)
			   && v < SZ(dis) && "invalid vertex index");
		return dis[u] + dis[v] - 2 * dis[query(u, v)];
	}
	/// @brief 全ての辺のコストを1とした時のuとvの距離を求める
	/// @param u クエリの頂点1
	/// @param v クエリの頂点2
	/// @return 距離
	int get_simple_dis(int u, int v) {
		assert(u >= 0 && v >= 0 && u < SZ(simple_dis)
			   && v < SZ(simple_dis)
			   && "invalid vertex index");
		return simple_dis[u] + simple_dis[v]
			- 2 * simple_dis[query(u, v)];
	}
};