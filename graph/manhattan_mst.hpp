#include <atcoder/all>
#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
using namespace atcoder;
PL op_for_manhattan_mst(PL a, PL b) {
	return min(a, b);
}
PL e_for_manhattan_mst() {
	return { LINF, -1 };
}

Graph<> manhattan_mst(vector<PL>& V) {
	int N = SZ(V);
	Graph<> G(N);
	if(N == 1) {
		return G;
	}
	Edges<> EG;
	auto sweep = [&]() {
		vll y;
		REP(i, N) {
			y.push_back(V[i].second);
		}
		so(y);
		uni(y);
		int y_len = SZ(y);
		map<ll, int> y_ind;
		REP(i, y_len) {
			y_ind[y[i]] = i;
		}
		segtree<PL, op_for_manhattan_mst,
				e_for_manhattan_mst>
			seg(y_len);
		vi p(N);
		REP(i, N) {
			p[i] = i;
		}
		sort(ALL(p), [=](auto const& l, auto const& r) {
			auto [lx, ly] = V[l];
			auto [rx, ry] = V[r];
			if(ly - lx != ry - rx) return ly - lx < ry - rx;
			if(ly != ry) return ly > ry;
			return l < r;
		});
		REP(i, N) {
			int v = p[i];
			auto [x, y] = V[v];
			PL d = seg.prod(y_ind[y], y_len);
			if(d != e_for_manhattan_mst()) {
				auto [nx, ny] = V[d.second];
				EG.eb(v, d.second,
					  abs(x - nx) + abs(y - ny));
			}
			PL c = seg.get(y_ind[y]);
			if(c.first > x + y) {
				seg.set(y_ind[y], { x + y, v });
			}
		}
	};
	REP(i, 2) {
		REP(j, 2) {
			REP(k, 2) {
				sweep();
				REP(l, N) {
					swap(V[l].first, V[l].second);
				}
			}
			REP(l, N) {
				V[l].first *= -1;
			}
		}
		REP(l, N) {
			V[l].second *= -1;
		}
	}
	dsu d(N);
	sort(ALL(EG), [](auto const& lhs, auto const& rhs) {
		if(lhs.cost != rhs.cost)
			return lhs.cost < rhs.cost;
		else if(lhs.from != rhs.from)
			return lhs.from < rhs.from;
		return lhs.to < rhs.to;
	});
	for(auto e: EG) {
		if(d.same(e.from, e.to)) continue;
		d.merge(e.from, e.to);
		G[e.from].eb(e.from, e.to, e.cost);
		G[e.to].eb(e.to, e.from, e.cost);
	}
	assert(d.size(0) == N);
	return G;
}