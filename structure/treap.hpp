#pragma once
#include "daylight/base.hpp"
#include "daylight/range.hpp"
template<class S, S (*op)(S, S), S (*e)(), class F,
		 S (*mapping)(F, S, int), F (*composition)(F, F),
		 F (*id)()>
struct Treap {
private:
	mt19937_64 mt;
	uniform_int_distribution<uint64_t> rand;
	vector<S> value, acc;
	vector<F> lazy;
	vll priority;
	vi cnt;
	vb lazy_rev;
	vi lch, rch;
	int new_node(S v, ll p) {
		value.push_back(v);
		acc.push_back(e());
		lazy.push_back(id());
		priority.push_back(p);
		cnt.push_back(0);
		lazy_rev.push_back(false);
		lch.push_back(-1);
		rch.push_back(-1);
		return SZ(value) - 1;
	}

	int root = -1;
	int get_cnt(int t) {
		return t == -1 ? 0 : cnt[t];
	}
	S get_acc(int t) {
		return t == -1 ? e() : acc[t];
	}
	int update(int t) {
		if(t == -1) return t;
		cnt[t] = 1 + get_cnt(lch[t]) + get_cnt(rch[t]);
		acc[t] = op(get_acc(lch[t]),
					op(value[t], get_acc(rch[t])));
		return t;
	}
	int push(int t) {
		if(t == -1) return t;
		if(lazy_rev[t]) {
			lazy_rev[t] = false;
			swap(lch[t], rch[t]);
			if(lch[t] != -1)
				lazy_rev[lch[t]] = !lazy_rev[lch[t]];
			if(rch[t] != -1)
				lazy_rev[rch[t]] = !lazy_rev[rch[t]];
		}
		if(lazy[t] != id()) {
			if(lch[t] != -1) {
				lazy[lch[t]]
					= composition(lazy[t], lazy[lch[t]]);
				acc[lch[t]] = mapping(lazy[t], acc[lch[t]],
									  get_cnt(lch[t]));
			}
			if(rch[t] != -1) {
				lazy[rch[t]]
					= composition(lazy[t], lazy[rch[t]]);
				acc[rch[t]] = mapping(lazy[t], acc[rch[t]],
									  get_cnt(rch[t]));
			}
			value[t] = mapping(lazy[t], value[t], 1);
			lazy[t] = id();
		}
		return update(t);
	}
	int apply(int t, int l, int r, F f) {
		auto [t1, tt] = split(t, l);
		auto [t2, t3] = split(tt, r - l);
		lazy[t2] = composition(f, lazy[t2]);
		acc[t2] = mapping(f, acc[t2], get_cnt(t2));
		return merge(merge(t1, t2), t3);
	}
	int merge(int l, int r) {
		push(l);
		push(r);
		if(l == -1) return r;
		if(r == -1) return l;
		if(priority[l] > priority[r]) {
			rch[l] = merge(rch[l], r);
			return update(l);
		} else {
			lch[r] = merge(l, lch[r]);
			return update(r);
		}
	}
	PI split(int t, int k) {
		if(t == -1) return make_pair(-1, -1);
		push(t);
		int implicit_key = get_cnt(lch[t]) + 1;
		if(k < implicit_key) {
			PI s = split(lch[t], k);
			lch[t] = s.second;
			return make_pair(s.first, update(t));
		} else {
			PI s = split(rch[t], k - implicit_key);
			rch[t] = s.first;
			return make_pair(update(t), s.second);
		}
	}
	int insert(int t, int k, int n) {
		auto s = split(t, k);
		return merge(merge(s.first, n), s.second);
	}
	int _erase(int t, int k) {
		auto [tt, t3] = split(t, k + 1);
		auto [t1, t2] = split(tt, k);
		return merge(t1, t3);
	}
	int erase_range(int t, int l, int r) {
		auto [tt, t3] = split(t, r);
		auto [t1, t2] = split(tt, l);
		return merge(t1, t3);
	}
	pair<S, int> query(int t, int l, int r) {
		auto [t1, tt] = split(t, l);
		auto [t2, t3] = split(tt, r - l);
		S ret = acc[t2];
		return make_pair(ret, merge(merge(t1, t2), t3));
	}
	int set(int t, int k, S v) {
		auto [tt, t3] = split(t, k + 1);
		auto [t1, t2] = split(tt, k);
		push(t2);
		value[t2] = v;
		update(t2);
		return merge(merge(t1, t2), t3);
	}
	int _find(int t, S x, int offset, bool left = true) {
		if(op(get_acc(t), x) == x) {
			return -1;
		} else {
			if(left) {
				if(lch[t] != -1
				   && op(acc[lch[t]], x) != x) {
					return find(lch[t], x, offset, left);
				} else {
					return (op(value[t], x) != x)
						? offset + get_cnt(lch[t])
						: find(rch[t], x,
							   offset + get_cnt(lch[t]) + 1,
							   left);
				}
			} else {
				if(rch[t] != -1
				   && op(acc[rch[t]], x) != x) {
					return find(
						rch[t], x,
						offset + get_cnt(lch[t]) + 1, left);
				} else {
					return (op(value[t], x) != x)
						? offset + get_cnt(lch[t])
						: find(lch[t], x, offset, left);
				}
			}
		}
	}
	int reverse(int t, int l, int r) {
		auto [t1, tt] = split(t, l);
		auto [t2, t3] = split(tt, r - l);
		lazy_rev[t2] = !lazy_rev[t2];
		return merge(merge(t1, t2), t3);
	}
	int rotate(int t, int l, int m, int r) {
		t = reverse(t, l, r);
		t = reverse(t, l, l + r - m);
		return reverse(t, l + r - m, r);
	}
	int lower_search(int t, S x) {
		int ret = 0;
		while(t != -1) {
			if(x <= value[t]) {
				t = lch[t];
			} else {
				ret += get_cnt(lch[t]) + 1;
				t = rch[t];
			}
		}
		return ret;
	}
	int upper_search(int t, S x) {
		int ret = 0;
		while(t != -1) {
			if(x < value[t]) {
				t = lch[t];
			} else {
				ret += get_cnt(lch[t]) + 1;
				t = rch[t];
			}
		}
		return ret;
	}

public:
	Treap() {
		mt = mt19937_64(chrono::steady_clock::now()
							.time_since_epoch()
							.count());
		rand = uniform_int_distribution<uint64_t>(1, 1e18);
	}
	/// @brief treapに追加された要素数を求める
	/// @return treapに追加された要素数
	size_t size() {
		return size_t(get_cnt(root));
	}
	/// @brief インデックスがindになるように要素xを追加する
	/// @param ind 追加先のインデックス
	/// @param x 追加する要素
	void insert(int ind, S x) {
		root = insert(root, ind, new_node(x, rand(mt)));
	}
	/// @brief 末尾に要素xを追加する
	/// @param x 追加する要素
	void push_back(S x) {
		root = insert(root, int(size()),
					  new_node(x, rand(mt)));
	}
	void ordered_insert(S x) {
		int ind = lower_search(root, x);
		insert(ind, x);
	}
	/// @brief (Ordered only)値が範囲[l,r)の中に存在するノードの数を取得
	/// @param l 値の範囲の下限(inclusive)
	/// @param r 値の範囲の上限(exclusive)
	/// @return 範囲内のノード数
	int value_range_cnt(Range<S> range) {
		int L = 0;
		if(range.getLeft().first) {
			S l = range.getLeft().second;
			if(range.isLeftInclusive()) {
				L = lower_search(root, l);
			} else {
				L = upper_search(root, l);
			}
		}
		int R = get_cnt(root);
		if(range.getRight().first) {
			S r = range.getRight().second;
			if(range.isRightInclusive()) {
				R = upper_search(root, r);
			} else {
				R = lower_search(root, r);
			}
		}
		return R - L;
	}
	/// @brief (Ordered Only)値が範囲[l,r)の中に存在するノードのaccを求める
	/// @param l 値の範囲の下限(inclusive)
	/// @param r 値の範囲の上限(exclusive)
	/// @return 範囲内のノード数
	S value_range_sum(Range<S> range) {
		int L = 0;
		if(range.getLeft().first) {
			S l = range.getLeft().second;
			if(range.isLeftInclusive()) {
				L = lower_search(root, l);
			} else {
				L = upper_search(root, l);
			}
		}
		int R = get_cnt(root);
		if(range.getRight().first) {
			S r = range.getRight().second;
			if(range.isRightInclusive()) {
				R = upper_search(root, r);
			} else {
				R = lower_search(root, r);
			}
		}
		if(L == R) return e();
		return query(L, R);
	}
	/// @brief (Ordered only)値がxであるような要素をcnt個削除する
	/// @param x 削除する要素の値
	/// @param cnt 削除する個数
	/// @return 実際に削除した個数
	int erase_value(S x, int cnt = -1) {
		int L = lower_search(root, x);
		int R = upper_search(root, x);
		if(cnt != -1) chmin(R, L + cnt);
		root = erase_range(root, L, R);
		return R - L;
	}

	/// @brief 値がx未満の要素の個数を求める
	/// @param x 境界値
	/// @return 条件を満たす個数
	int lower_search(S x) {
		return lower_search(root, x);
	}
	/// @brief 値がx以上の要素の個数を求める
	/// @param x 境界値
	/// @return 条件を満たす個数
	int upper_search(S x) {
		return upper_search(root, x);
	}

	/// @brief UpdateMonoidであるfを範囲[l,r)に適応する
	/// @param l 範囲の左端(inclusive)
	/// @param r 範囲の右端(exclusive)
	/// @param f UpdateMonoidの要素f
	void apply(int l, int r, F f) {
		root = apply(root, l, r, f);
	}
	/// @brief インデックスindの要素を削除する
	/// @param ind 削除する要素のインデックス
	void erase(int ind) {
		root = _erase(root, ind);
	}
	/// @brief インデックスが[l,r)の区間内にある要素を削除する
	/// @param l インデックスの左端
	/// @param r インデックスの右端
	void erase(int l, int r) {
		auto [tt, t3] = split(root, r);
		auto [t1, t2] = split(tt, l);
		root = merge(t1, t3);
	}
	/// @brief 範囲[l,r)を反転させる
	/// @param l 範囲の左端(inclusive)
	/// @param r 範囲の右端(exclusive)
	void reverse(int l, int r) {
		root = reverse(root, l, r);
	}
	/// @brief 範囲[l,r)をインデックスmの要素が先頭になるように回転させる
	/// @param l 範囲の左端(inclusive)
	/// @param m 回転後の先頭要素のインデックス
	/// @param r 範囲の右端(exclusive)
	void rotate(int l, int m, int r) {
		root = rotate(root, l, m, r);
	}
	/// @brief インデックスkの要素にvを代入する
	/// @param k 更新する要素k
	/// @param v 更新後の要素v
	void set(int k, S v) {
		root = set(root, k, v);
	}
	/// @brief Monoid::op(value[k],x)!=xになるような[l,r)内のkで最左/最右を求める.
	/// @param l 探索範囲の左端(inclusive)
	/// @param r 探索範囲の右端(exclusive)
	/// @param x 探索対象の要素x
	/// @param left 最左/最右を指定
	/// @return 条件を満たすk
	int find(int l, int r, S x, bool left = true) {
		auto [t1, tt] = split(root, l);
		auto [t2, t3] = split(tt, r - l);
		int ret = _find(t2, x, l, left);
		if(ret == -1) ret = r;
		root = merge(merge(t1, t2), t3);
		return ret;
	}
	/// @brief Monoid::op(value[l,r))を求める
	/// @param l 範囲の左端(inclusive)
	/// @param r 範囲の右端(exclusive)
	/// @return 範囲の演算結果を求める
	S query(int l, int r) {
		auto [t, rt] = query(root, l, r);
		root = rt;
		return t;
	}
	/// @brief value[ind]を求める
	/// @param ind 取得するindex
	/// @return value[ind]
	S operator[](int ind) {
		auto [tt, t3] = split(root, ind + 1);
		auto [t1, t2] = split(tt, ind);
		S ret = acc[t2];
		root = merge(merge(t1, t2), t3);
		return ret;
	}
};