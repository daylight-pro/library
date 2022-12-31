#include "daylight/base.hpp"
#include "daylight/math/powmod.hpp"
/// @brief Modint用の階乗計算関数です。
/// @tparam T modintの型
/// @param N 求める階乗の最大値
/// @param fact out:階乗
template<class T>
void setFact(int N, vector<T> &fact) {
	fact = vector<T>(N + 1);
	fact[0] = T(1);
	FOR(i, 1, N + 1) {
		fact[i] = fact[i - 1] * i;
	}
}
/// @brief ll用の階乗およびその逆元計計算関数で。
/// @param N 求める会場の最大値
/// @param fact out:階乗
/// @param inv out:階乗の逆元
void setFact(int N, vll &fact, vll &inv) {
	fact = vector<ll>(N + 1);
	inv = vector<ll>(N + 1);
	fact[0] = 1;
	FOR(i, 1, N + 1) {
		fact[i] = (fact[i - 1] * i) % MOD;
	}
	inv[N] = powmod(fact[N], MOD - 2);
	REPR(i, N) {
		inv[i] = inv[i + 1] * (i + 1) % MOD;
	}
}