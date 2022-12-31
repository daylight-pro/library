#include "daylight/base.hpp"
template<typename T, const T e>
class cht {
	struct line {
		T a, b;
		line(T a = 0, T b = e): a(a), b(b) {
		}
		T get(T x) {
			return a * x + b;
		}
	};
	vector<line> V;
	vector<T> X;
	int sz;

	void _add_line(line L, int k, int l, int r) {
		int m = (l + r) / 2;
		T lx = X[l], mx = X[m], rx = X[r - 1];
		bool left = (L.get(lx) < V[k].get(lx));
		bool mid = (L.get(mx) < V[k].get(mx));
		bool right = (L.get(rx) < V[k].get(rx));
		if(left && right) {
			//区間内で新しい線のほうが常に小さい
			V[k] = L;
			return;
		}
		if(!left && !right) {
			//区間内で古い線のほうが常に小さい
			return;
		}
		if(mid) {
			//対称性を利用するために、入れ替え
			swap(L, V[k]);
		}
		if(left != mid) {
			//区間の左半分で交わる
			_add_line(L, 2 * k + 1, l, m);
		} else {
			//区間の右半分で交わる
			_add_line(L, 2 * k + 2, m, r);
		}
	}

public:
	/// @brief コンストラクタ
	/// @param _X クエリのx座標の配列
	/// @param fill _Xの余った部分を埋める値(max(_X)の数倍程度で、オーバーフローしない値)
	cht(vector<T> const& _X, T fill): X(_X) {
		int N = SZ(X);
		int N0 = 1;
		if(__builtin_popcount(N) == 1) N--;
		while(N > 0) {
			N >>= 1;
			N0 <<= 1;
		}
		sz = N0;
		while(SZ(X) < sz) {
			X.push_back(fill);
		}
		V.resize(sz * 2 + 1);
	}
	/// @brief f(X[k])の最小値を求める。
	/// @param k クエリ集合でのX座標のインデックス
	/// @return f(X[k])の最小値
	T query(int k) {
		T x = X[k];
		k += sz - 1;
		T ret = e;
		while(true) {
			chmin(ret, V[k].get(x));
			if(k == 0) break;
			k = (k - 1) / 2;
		}
		return ret;
	}

	/// @brief ax+bとなる直線を追加します。
	/// @param a 直線の傾き
	/// @param b 直線のy切片
	void add_line(T a, T b) {
		_add_line(line(a, b), 0, 0, sz);
	}
};