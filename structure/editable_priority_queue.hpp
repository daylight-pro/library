#include "atcoder/all"
#include "daylight/base.hpp"
using namespace atcoder;
namespace inner_editable_pq {
pair<ll, int> op(pair<ll, int> a, pair<ll, int> b) {
	return min(a, b);
}
pair<ll, int> e() {
	return { LINF, -1 };
}
}
struct editable_priority_queue {
private:
	segtree<pair<ll, int>, inner_editable_pq::op,
			inner_editable_pq::e>
		seg;
	int sz;
	int N;

public:
	editable_priority_queue(vll& v) {
		sz = SZ(v);
		N = SZ(v);
		vector<pair<ll, int>> V;
		REP(i, N) {
			if(v[i] == inner_editable_pq::e().first) {
				V.emplace_back(v[i], -1);
			} else {
				V.emplace_back(v[i], i);
			}
		}
		seg = segtree<pair<ll, int>, inner_editable_pq::op,
					  inner_editable_pq::e>(V);
	}
	/// @brief 配列のi番目の値にアクセスする
	/// @param i アクセスする配列のインデックス(0-indexed)
	/// @return 配列のi番目の値
	ll get(int i) {
		auto [a, b] = seg.get(i);
		return a;
	}
	/// @brief 配列のi番目の値をxに変更する
	/// @param i 変更する配列のインデックス(0-indexed)
	/// @param x 変更後の値
	void set(int i, ll x) {
		auto [a, b] = seg.get(i);
		if(b == -1) sz++;
		if(x == inner_editable_pq::e().first) {
			seg.set(i, { x, -1 });
			sz--;
		} else {
			seg.set(i, { x, i });
		}
	}
	/// @brief 配列の[0,size)の中で最小の値求める(同率の場合はインデックス最小)
	/// @return {最小の要素,最小要素のインデックス}
	pair<ll, int> top() {
		return top(0, N);
	}
	/// @brief 配列の[0,r)の中で最小の値求める(同率の場合はインデックス最小)
	/// @param r 探索範囲の右端(exclusive)
	/// @return {最小の要素,最小要素のインデックス}
	pair<ll, int> top(int r) {
		return top(0, r);
	}
	/// @brief 配列の[l,r)の中で最小の値を求める(同率の場合はインデックス最小)
	/// @param l 探索範囲の左端(inclusive)
	/// @param r 探索範囲の右端(exclusive)
	/// @return {最小の要素,最小要素のインデックス}
	pair<ll, int> top(int l, int r) {
		auto [a, b] = seg.prod(l, r);
		return { a, b };
	}
	/// @brief 配列の[0,size)の中で最小の要素を削除する(同率の場合はインデックス最小)
	void pop() {
		return pop(0, N);
	}
	/// @brief 配列の[0,r)の中で最小の値を削除する(同率の場合はインデックス最小)
	/// @param r 探索範囲の右端(exclusive)
	void pop(int r) {
		return pop(0, r);
	}
	/// @brief 配列の[l,r)の中で最小の値を削除する(同率の場合はインデックス最小)
	/// @param l 探索範囲の左端(inclusive)
	/// @param r 探索範囲の右端(exclusive)
	void pop(int l, int r) {
		auto [a, b] = seg.prod(l, r);
		if(b == -1) {
			return;
		}
		seg.set(b, inner_editable_pq::e());
		sz--;
	}
	/// @brief 配列の要素がすべて削除されているか調べる
	/// @return 配列の要素がすべて削除されているならtrue,そうでなければfalse
	bool empty() {
		return sz == 0;
	}
	/// @brief 配列のi番目の要素が削除されているか調べる
	/// @param i 削除されているか調べるインデックス(0-indexed)
	/// @return 配列のi番目の要素が削除されているならtrue,そうでなければfalse
	bool is_removed(int i) {
		auto p = seg.get(i);
		return p == inner_editable_pq::e();
	}
	/// @brief 配列の削除されていない要素の数を返す
	/// @return 配列の要素数
	size_t size() {
		return sz;
	}
};