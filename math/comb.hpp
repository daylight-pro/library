#include "daylight/base.hpp"
template<typename T>
struct Comb {
private:
	vector<T> _fact;
	vector<T> inv;
	int pre_build;
	void expand(int x) {
		if(pre_build == -1) return;
		while(SZ(_fact) <= x) {
			int next = SZ(_fact);
			_fact.push_back(_fact[next - 1] * next);
			inv.push_back(_fact[next].inv());
		}
	}

	T nPr_calc(int n, int r) {
		T ret = 1;
		for(int i = n; i > n - r; i--) ret *= i;
		return ret;
	}
	T nCr_calc(int n, int r) {
		T ret = 1;
		REP(i, r) {
			ret *= n - i;
			ret /= i + 1;
		}
		return ret;
	}

public:
	Comb(int pre_build = 0): pre_build(pre_build) {
		if(pre_build == -1) {
			return;
		}
		_fact = vector<T>(pre_build + 1);
		_fact[0] = 1;
		inv = vector<T>(pre_build + 1);
		REP(i, pre_build) {
			_fact[i + 1] = _fact[i] * (i + 1);
		}
		inv[pre_build] = _fact[pre_build].inv();
		REPR(i, pre_build) {
			inv[i] = inv[i + 1] * (i + 1);
		}
	}
	T nPr(int n, int r) {
		assert(r >= 0);
		assert(n >= r);
		expand(n);
		if(pre_build == -1) return nPr_calc(n, r);
		return _fact[n] * inv[n - r];
	}
	T nCr(int n, int r) {
		assert(r >= 0);
		assert(n >= r);
		expand(n);
		if(pre_build == -1) return nCr_calc(n, r);
		return _fact[n] * inv[r] * inv[n - r];
	}
	T nHr(int n, int r) {
		assert(r >= 0);
		assert(n >= 1);
		expand(n + r - 1);
		return nCr(n + r - 1, r);
	}
	T type_permission(vi V) {
		assert(pre_build != -1);
		int sum = 0;
		for(int i: V) {
			assert(i >= 0);
			sum += i;
		}
		expand(sum);
		T ans = _fact[sum];
		for(int i: V) ans *= inv[i];
		return ans;
	}
	T fact(int n) {
		expand(n);
		return _fact[n];
	}
	T inv_fact(int n) {
		expand(n);
		return inv[n];
	}
};