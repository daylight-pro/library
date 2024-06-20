#include "daylight/base.hpp"
using UPDATE = function<void(int)>;
using ADD = function<void(int, bool)>;
using DEL = function<void(int, bool)>;
using RESET = function<void(void)>;
using SNAPSHOT = function<void(void)>;
using ROLLBACK = function<void(void)>;

/// @brief 区間クエリに高速にこたえる
/// @tparam T クエリの答えの型
/// @param N 区間の最大長
/// @param queries クエリの配列
/// @param update (クエリ番号)->現在の区間の答え
/// @param add (インデックス,左からの操作かどうか)
/// @param del (インデックス,左からの操作かどうか)
/// @return クエリの答えの配列
inline void Mo_s_algorithm(int N,
						   vector<pair<int, int>>& queries,
						   UPDATE update, ADD add,
						   DEL del) {
	int Q = SZ(queries);
	if(Q == 0) return;
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
		if(queries[i].second != queries[j].second)
			return queries[i].second < queries[j].second;
		return queries[i] < queries[j];
	});
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
		update(i);
	}
}

inline void Rollback_Mo(int N,
						vector<pair<int, int>>& queries,
						UPDATE update, ADD add, RESET reset,
						SNAPSHOT snapshot,
						ROLLBACK rollback) {
	int Q = SZ(queries);
	if(Q == 0) return;
	int len = max(1, int(N / sqrt(Q)));
	vi Qi(Q);
	iota(ALL(Qi), 0);
	sort(ALL(Qi), [&](int i, int j) {
		int bi = queries[i].first / len;
		int bj = queries[j].first / len;
		if(bi != bj) return bi < bj;
		if(queries[i].second != queries[j].second)
			return queries[i].second < queries[j].second;
		return i < j;
	});
	for(auto q: Qi) {
		auto [l, r] = queries[q];
		if(r - l <= len) {
			FOR(i, l, r) add(i, false);
			update(q);
			rollback();
		}
	}
	int pre = -1, curR = 0;
	for(auto q: Qi) {
		auto [l, r] = queries[q];
		if(r - l <= len) continue;
		int cur = l / len;
		if(pre < cur) {
			reset();
			pre = cur;
			curR = (cur + 1) * len;
		}
		while(curR < r) {
			add(curR, false);
			curR++;
		}
		snapshot();
		for(int i = (cur + 1) * len - 1; i >= l; i--) {
			add(i, true);
		}
		update(q);
		rollback();
	}
}