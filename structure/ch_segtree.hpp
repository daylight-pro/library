#include "daylight/base.hpp"
class SegmentTreeBeats {
private:
	const ll inf = 1e18;
	int n, n0;
	vll max_v, smax_v, max_c;
	vll min_v, smin_v, min_c;
	vll sum;
	vll len, ladd, lval;

	void update_node_max(int k, ll x) {
		sum[k] += (x - max_v[k]) * max_c[k];

		if(max_v[k] == min_v[k]) {
			max_v[k] = min_v[k] = x;
		} else if(max_v[k] == smin_v[k]) {
			max_v[k] = smin_v[k] = x;
		} else {
			max_v[k] = x;
		}

		if(lval[k] != inf && x < lval[k]) {
			lval[k] = x;
		}
	}
	void update_node_min(int k, ll x) {
		sum[k] += (x - min_v[k]) * min_c[k];

		if(max_v[k] == min_v[k]) {
			max_v[k] = min_v[k] = x;
		} else if(smax_v[k] == min_v[k]) {
			min_v[k] = smax_v[k] = x;
		} else {
			min_v[k] = x;
		}

		if(lval[k] != inf && lval[k] < x) {
			lval[k] = x;
		}
	}

	void push(int k) {
		if(n0 - 1 <= k) return;

		if(lval[k] != inf) {
			updateall(2 * k + 1, lval[k]);
			updateall(2 * k + 2, lval[k]);
			lval[k] = inf;
			return;
		}

		if(ladd[k] != 0) {
			addall(2 * k + 1, ladd[k]);
			addall(2 * k + 2, ladd[k]);
			ladd[k] = 0;
		}

		if(max_v[k] < max_v[2 * k + 1]) {
			update_node_max(2 * k + 1, max_v[k]);
		}
		if(min_v[2 * k + 1] < min_v[k]) {
			update_node_min(2 * k + 1, min_v[k]);
		}

		if(max_v[k] < max_v[2 * k + 2]) {
			update_node_max(2 * k + 2, max_v[k]);
		}
		if(min_v[2 * k + 2] < min_v[k]) {
			update_node_min(2 * k + 2, min_v[k]);
		}
	}

	void update(int k) {
		sum[k] = sum[2 * k + 1] + sum[2 * k + 2];

		if(max_v[2 * k + 1] < max_v[2 * k + 2]) {
			max_v[k] = max_v[2 * k + 2];
			max_c[k] = max_c[2 * k + 2];
			smax_v[k]
				= max(max_v[2 * k + 1], smax_v[2 * k + 2]);
		} else if(max_v[2 * k + 1] > max_v[2 * k + 2]) {
			max_v[k] = max_v[2 * k + 1];
			max_c[k] = max_c[2 * k + 1];
			smax_v[k]
				= max(smax_v[2 * k + 1], max_v[2 * k + 2]);
		} else {
			max_v[k] = max_v[2 * k + 1];
			max_c[k] = max_c[2 * k + 1] + max_c[2 * k + 2];
			smax_v[k]
				= max(smax_v[2 * k + 1], smax_v[2 * k + 2]);
		}

		if(min_v[2 * k + 1] < min_v[2 * k + 2]) {
			min_v[k] = min_v[2 * k + 1];
			min_c[k] = min_c[2 * k + 1];
			smin_v[k]
				= min(smin_v[2 * k + 1], min_v[2 * k + 2]);
		} else if(min_v[2 * k + 1] > min_v[2 * k + 2]) {
			min_v[k] = min_v[2 * k + 2];
			min_c[k] = min_c[2 * k + 2];
			smin_v[k]
				= min(min_v[2 * k + 1], smin_v[2 * k + 2]);
		} else {
			min_v[k] = min_v[2 * k + 1];
			min_c[k] = min_c[2 * k + 1] + min_c[2 * k + 2];
			smin_v[k]
				= min(smin_v[2 * k + 1], smin_v[2 * k + 2]);
		}
	}

	void _update_min(ll x, int a, int b, int k, int l,
					 int r) {
		if(b <= l || r <= a || max_v[k] <= x) {
			return;
		}
		if(a <= l && r <= b && smax_v[k] < x) {
			update_node_max(k, x);
			return;
		}

		push(k);
		_update_min(x, a, b, 2 * k + 1, l, (l + r) / 2);
		_update_min(x, a, b, 2 * k + 2, (l + r) / 2, r);
		update(k);
	}

	void _update_max(ll x, int a, int b, int k, int l,
					 int r) {
		if(b <= l || r <= a || x <= min_v[k]) {
			return;
		}
		if(a <= l && r <= b && x < smin_v[k]) {
			update_node_min(k, x);
			return;
		}

		push(k);
		_update_max(x, a, b, 2 * k + 1, l, (l + r) / 2);
		_update_max(x, a, b, 2 * k + 2, (l + r) / 2, r);
		update(k);
	}

	void addall(int k, ll x) {
		max_v[k] += x;
		if(smax_v[k] != -inf) smax_v[k] += x;
		min_v[k] += x;
		if(smin_v[k] != inf) smin_v[k] += x;

		sum[k] += len[k] * x;
		if(lval[k] != inf) {
			lval[k] += x;
		} else {
			ladd[k] += x;
		}
	}

	void updateall(int k, ll x) {
		max_v[k] = x;
		smax_v[k] = -inf;
		min_v[k] = x;
		smin_v[k] = inf;
		max_c[k] = min_c[k] = len[k];

		sum[k] = x * len[k];
		lval[k] = x;
		ladd[k] = 0;
	}

	void _add_val(ll x, int a, int b, int k, int l, int r) {
		if(b <= l || r <= a) {
			return;
		}
		if(a <= l && r <= b) {
			addall(k, x);
			return;
		}

		push(k);
		_add_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
		_add_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
		update(k);
	}

	void _update_val(ll x, int a, int b, int k, int l,
					 int r) {
		if(b <= l || r <= a) {
			return;
		}
		if(a <= l && r <= b) {
			updateall(k, x);
			return;
		}

		push(k);
		_update_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
		_update_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
		update(k);
	}

	ll _query_max(int a, int b, int k, int l, int r) {
		if(b <= l || r <= a) {
			return -inf;
		}
		if(a <= l && r <= b) {
			return max_v[k];
		}
		push(k);
		ll lv = _query_max(a, b, 2 * k + 1, l, (l + r) / 2);
		ll rv = _query_max(a, b, 2 * k + 2, (l + r) / 2, r);
		return max(lv, rv);
	}

	ll _query_min(int a, int b, int k, int l, int r) {
		if(b <= l || r <= a) {
			return inf;
		}
		if(a <= l && r <= b) {
			return min_v[k];
		}
		push(k);
		ll lv = _query_min(a, b, 2 * k + 1, l, (l + r) / 2);
		ll rv = _query_min(a, b, 2 * k + 2, (l + r) / 2, r);
		return min(lv, rv);
	}

	ll _query_sum(int a, int b, int k, int l, int r) {
		if(b <= l || r <= a) {
			return 0;
		}
		if(a <= l && r <= b) {
			return sum[k];
		}
		push(k);
		ll lv = _query_sum(a, b, 2 * k + 1, l, (l + r) / 2);
		ll rv = _query_sum(a, b, 2 * k + 2, (l + r) / 2, r);
		return lv + rv;
	}

public:
	SegmentTreeBeats(int N): SegmentTreeBeats(vll(N)) {
	}

	/// @brief セグ木を配列の内容で初期化する
	/// @param a セグ木の初期値
	SegmentTreeBeats(const vll &a) {
		n = SZ(a);
		n0 = 1;
		while(n0 < n) n0 <<= 1;

		max_v = vll(n0 * 2);
		smax_v = vll(n0 * 2), max_c = vll(n0 * 2);
		min_v = vll(n0 * 2);
		smin_v = vll(n0 * 2), min_c = vll(n0 * 2);
		sum = vll(n0 * 2);
		len = vll(n0 * 2), ladd = vll(n0 * 2),
		lval = vll(n0 * 2);
		for(int i = 0; i < 2 * n0; ++i)
			ladd[i] = 0, lval[i] = inf;
		len[0] = n0;
		for(int i = 0; i < n0 - 1; ++i)
			len[2 * i + 1] = len[2 * i + 2] = (len[i] >> 1);

		for(int i = 0; i < n; ++i) {
			max_v[n0 - 1 + i] = min_v[n0 - 1 + i]
				= sum[n0 - 1 + i] = a[i];
			smax_v[n0 - 1 + i] = -inf;
			smin_v[n0 - 1 + i] = inf;
			max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 1;
		}

		for(int i = n; i < n0; ++i) {
			max_v[n0 - 1 + i] = smax_v[n0 - 1 + i] = -inf;
			min_v[n0 - 1 + i] = smin_v[n0 - 1 + i] = inf;
			max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 0;
		}
		for(int i = n0 - 2; i >= 0; i--) {
			update(i);
		}
	}

	/// @brief p番目の要素にアクセス
	/// @param p アクセスする要素のインデックス(0-indexed)
	/// @return p番目の要素
	ll get(int p) {
		return prod_sum(p, p + 1);
	}

	/// @brief p番目の要素をxに変更
	/// @param p 変更する要素のインデックス(0-indexed)
	/// @param x 変更後の値
	void set(int p, ll x) {
		apply_update(p, p + 1, x);
	}

	/// @brief 区間[a,b)でv[i]←min(v[i],x)
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @param x minをとる値
	void apply_chmin(int a, int b, ll x) {
		_update_min(x, a, b, 0, 0, n0);
	}

	/// @brief 区間[a,b)でv[i]←max(v[i],x)
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @param x maxをとる値
	void apply_chmax(int a, int b, ll x) {
		_update_max(x, a, b, 0, 0, n0);
	}

	/// @brief 区間[a,b)でv[i]←v[i]+x
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @param x 加算する値
	void apply_add(int a, int b, ll x) {
		_add_val(x, a, b, 0, 0, n0);
	}

	/// @brief 区間[a,b)でv[i]←x
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @param x 更新する値
	void apply_update(int a, int b, ll x) {
		_update_val(x, a, b, 0, 0, n0);
	}

	/// @brief 区間[a,b)の最大値を取得
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @return 区間[a,b)の最大値
	ll prod_max(int a, int b) {
		return _query_max(a, b, 0, 0, n0);
	}

	/// @brief 区間[a,b)の最小値を取得
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @return 区間[a,b)の最小値
	ll prod_min(int a, int b) {
		return _query_min(a, b, 0, 0, n0);
	}

	/// @brief 区間[a,b)の和を取得
	/// @param a 区間の左端(inclusive)
	/// @param b 区間の右端(exclusive)
	/// @return 区間[a,b)の和
	ll prod_sum(int a, int b) {
		return _query_sum(a, b, 0, 0, n0);
	}

	/// @brief 全体の最大値を取得
	/// @return 全体の最大値
	ll all_prod_max() {
		return prod_max(0, n);
	}

	/// @brief 全体の最小値を取得
	/// @return 全体の最小値
	ll all_prod_min() {
		return prod_min(0, n);
	}

	/// @brief 全体の総和を取得
	/// @return 全体の総和
	ll all_prod_sum() {
		return prod_sum(0, n);
	}
};