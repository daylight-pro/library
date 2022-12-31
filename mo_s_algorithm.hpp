#include "daylight/base.hpp"
/// @brief 区間クエリに高速にこたえる
/// @tparam T クエリの答えの型
/// @param N 区間の最大長
/// @param queries クエリの配列
/// @param get (クエリ番号)->現在の区間の答え
/// @param add (インデックス,左からの操作かどうか)
/// @param del (インデックス,左からの操作かどうか)
/// @return クエリの答えの配列
template<typename T, typename F1, typename F2>
inline vector<T> Mo_s_algorithm(
	int N, vector<pair<int, int>>& queries, F1 get, F2 add,
	F2 del) {
	int Q = SZ(queries);
	if(Q == 0) return vector<T>(0);
	int curL = 0;
	int curR = 0;
	vi Qi(Q);
	iota(ALL(Qi), 0);
	sort(ALL(Qi), [&](int i, int j) {
		int bi
			= queries[i].first / max(1, int(N / sqrt(Q)));
		int bj
			= queries[j].first / max(1, int(N / sqrt(Q)));
		if(bi != bj) return bi < bj;
		return queries[i].second < queries[j].second;
	});
	vector<T> ret(Q, T(0));
	for(auto i: Qi) {
		auto [l, r] = queries[i];
		while(curR < r) {
			add(curR, false);
			curR++;
		}
		while(curL < l) {
			del(curL, true);
			curL++;
		}
		while(r < curR) {
			curR--;
			del(curR, false);
		}
		while(l < curL) {
			curL--;
			add(curL, true);
		}
		ret[i] = get(i);
	}
	return ret;
}
