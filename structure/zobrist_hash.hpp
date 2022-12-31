#include "atcoder/all"
#include "daylight/base.hpp"
using namespace atcoder;
/// @brief 集合をハッシュする構造体
/// @tparam T 配列の要素の型
template<typename T>
struct ZobristHash {
private:
	map<T, uint64_t> M1;
	map<T, uint64_t> M2;

	void createRand(const vector<T>& t) {
		mt19937_64 mt(chrono::steady_clock::now()
						  .time_since_epoch()
						  .count());
		uniform_int_distribution<uint64_t> rand(1,
												1ll << 62);
		for(auto s: t) {
			if(M1[s] != 0) continue;
			M1[s] = rand(mt);
			M2[s] = rand(mt);
		}
	}

public:
	ZobristHash() {
	}
	/// @brief queriesの各クエリについて,tの集合のハッシュを求める
	/// @param t ハッシュを求める対象の配列
	/// @param queries ハッシュを求める範囲[l,r)のペア配列
	/// @return 0<=u<SZ(queries)について,ret[i]は[first,second)の集合としてのハッシュが入る
	vector<pair<uint64_t, uint64_t>> get(
		vector<T> const& t,
		vector<pair<int, int>> const& queries) {
		createRand(t);
		vector<pair<pair<int, int>, int>> V;
		REP(i, SZ(queries)) {
			auto [l, r] = queries[i];
			assert(0 <= l && l < r && r <= SZ(t));
			V.push_back({ { l, r }, i });
		}
		so(V);
		int N = SZ(t);
		auto op = [](uint64_t a, uint64_t b) -> uint64_t {
			return a ^ b;
		};
		auto e = []() -> uint64_t {
			return 0;
		};
		vector<uint64_t> init1(N, e());
		vector<uint64_t> init2(N, e());
		vi next(N, -1);
		map<T, int> pre;
		REP(i, N) {
			if(pre[t[i]] != 0) {
				next[pre[t[i]] - 1] = i;
			} else {
				init1[i] = M1[t[i]];
				init2[i] = M2[t[i]];
			}
			pre[t[i]] = i + 1;
		}
		segtree<uint64_t, op, e> seg1(init1);
		segtree<uint64_t, op, e> seg2(init2);
		int curL = 0;
		vector<pair<uint64_t, uint64_t>> ret(SZ(queries));
		REP(i, SZ(V)) {
			auto [q, ind] = V[i];
			auto [l, r] = q;
			while(curL < l) {
				seg1.set(curL, e());
				seg2.set(curL, e());
				if(next[curL] != -1) {
					seg1.set(next[curL], M1[t[next[curL]]]);
					seg2.set(next[curL], M2[t[next[curL]]]);
				}
				curL++;
			}
			ret[ind] = { seg1.prod(l, r), seg2.prod(l, r) };
		}
		return ret;
	}
};