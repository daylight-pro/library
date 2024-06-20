#include "daylight/base.hpp"
template<class T = ll>
struct Doubling {
private:
	int N;
	vvi V;
	vector<vector<T>> S;
	vector<T> s;
	function<T(T, T)> op;
	function<T()> e;
	bool hasMonoid = false;

	inline void build_next() {
		int n = SZ(V);
		V.push_back(vi(N));
		if(hasMonoid) S.push_back(vector<T>(N));
		REP(i, N) {
			V[n][i] = V[n - 1][V[n - 1][i]];
			if(hasMonoid)
				S[n][i] = op(S[n - 1][i],
							 S[n - 1][V[n - 1][i]]);
		}
	}

public:
	Doubling() {
	}

	/// @brief サイズnの恒等写像で初期化
	/// @param n ダブリングのサイズ
	Doubling(int n) {
		vi v(n);
		REP(i, n) v[i] = i;
		init(v);
	}

	/// @brief 写像を指定して初期化
	/// @param v ダブリングの写像
	Doubling(const vi& v) {
		init(v);
	}

	void init(const vi& v) {
		N = SZ(v);
		REP(i, N) {
			assert(v[i] >= 0 && v[i] < N);
		}
		V.push_back(vi(N));
		REP(i, N) V[0][i] = v[i];
	}
	/// @brief ダブリングにモノイドを載せる
	/// @param _s _s[i]はiから延びる辺に対応するモノイド
	/// @param _op モノイドの演算
	/// @param _e モノイドの単位元
	void setMonoid(const vector<T>& _s,
				   const function<T(T, T)>& _op,
				   const function<T()>& _e) {
		assert(SZ(_s) == N);
		hasMonoid = true;
		op = _op;
		e = _e;
		s = _s;
		S.clear();
		S.push_back(vector<T>(N));
		REP(i, N) S[0][i] = s[i];
	}

	/// @brief xをL回写像した時の値を求める
	/// @param L 写像の繰り返し回数
	/// @param x 写像の初期値
	/// @return xをL回写像した時の値
	int get(ll L, int x) {
		int ret = x;
		for(int i = 0; L > 0ll; i++) {
			if(i >= SZ(V)) build_next();
			if(L & 1ll) ret = V[i][ret];
			L >>= 1ll;
		}
		return ret;
	}

	/// @brief L回の写像でたどった辺に対応するモノイドの総積を求める
	/// @param L 写像の繰り返し回数
	/// @param x 写像の初期値
	/// @return L回の写像でたどった辺に対応するモノイド積
	T prod(ll L, int x) {
		assert(hasMonoid);
		int cur = x;
		T ret = e();
		for(int i = 0; L > 0ll; i++) {
			if(i >= SZ(V)) build_next();
			if(L & 1ll) {
				ret = op(ret, S[i][cur]);
				cur = V[i][cur];
			}
			L >>= 1ll;
		}
		return ret;
	}
	ll max_search(int s, function<bool(T)> f) {
		ll ok = 0;
		ll ng = 1;
		while(f(prod(ng, s))) {
			ng *= 2;
		}
		while(ng - ok > 1) {
			ll mid = (ng + ok) / 2;
			if(f(prod(mid, s))) {
				ok = mid;
			} else {
				ng = mid;
			}
		}
		return ok;
	}
	ll min_search(int s, function<bool(T)> f,
				  ll limit = LINF) {
		ll ng = -1;
		ll ok = 1;
		while(!f(prod(ok, s))) {
			if(ok * 2 >= LINF) {
				ok = LINF;
				break;
			}
			ok *= 2;
		}
		while(ok - ng > 1) {
			ll mid = (ok + ng) / 2;
			if(f(prod(mid, s))) {
				ok = mid;
			} else {
				ng = mid;
			}
		}
		return ok;
	}
};