#include "atcoder/all"
#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
using namespace atcoder;
/// @brief 最小全域木のコストを求める
/// @param V 頂点数
/// @param ES 辺集合
/// @return グラフが連結でなければLINF,連結ならば最小全域木のコスト
ll kruskal(int V, Edges<>& ES) {
	sort(ALL(ES), [](Edge<> const& a, Edge<> const& b) {
		if(a.cost != b.cost) return a.cost < b.cost;
		if(a.from != b.from) return a.from < b.from;
		return a.to < b.to;
	});
	ll ret = 0;
	dsu d(V);
	REP(i, SZ(ES)) {
		Edge e = ES[i];
		if(!d.same(e.from, e.to)) {
			ret += e.cost;
			d.merge(e.from, e.to);
		}
	}
	if(d.size(0) != V) {
		return LINF;
	}
	return ret;
}