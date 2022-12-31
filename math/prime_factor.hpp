#include "daylight/base.hpp"
/// @brief 素因数分解をする
/// @param n 素因数分解する整数
/// @return map[p]=a:p^aが因数に含まれる
map<ll, int> prime_factor(ll n) {
	map<ll, int> ret;
	for(ll i = 2; i * i <= n; i++) {
		while(n % i == 0) {
			ret[i]++;
			n /= i;
		}
	}
	if(n != 1) ret[n] = 1;
	return ret;
}
/// @brief 高速素因数分解の前計算を行う
/// @param n 高速素因数分解を行う整数の最大値
/// @return 高速素因数分解用の篩配列
vi sieve(int n) {
	n++;
	vi res(n);
	std::iota(res.begin(), res.end(), 0);
	for(int i = 2; i * i < n; ++i) {
		if(res[i] < i) continue;
		for(int j = i * i; j < n; j += i)
			if(res[j] == j) res[j] = i;
	}
	return res;
}

/// @brief 前計算した篩配列を用いて高速素因数分解
/// @param n 素因数分解する整数
/// @param min_factor 前計算した篩配列
/// @return map[p]=a:p^aが因数に含まれる
map<int, int> factor(int n, const vi& min_factor) {
	map<int, int> res;
	while(n > 1) {
		res[min_factor[n]]++;
		n /= min_factor[n];
	}
	return res;
}