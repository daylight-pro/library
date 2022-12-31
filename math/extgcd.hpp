#include "daylight/base.hpp"
/// @brief gcd(a,b)とax+by=gcd(a,b)を満たすx,yを求める
/// @param a 入力1つ目
/// @param b 入力2つ目
/// @param x 出力1つ目
/// @param y 出力2つ目
/// @return gcd(a,b)
ll extgcd(ll a, ll b, ll& x, ll& y) {
	ll d = a;
	if(b != 0) {
		d = extgcd(b, a % b, y, x);
		y -= (a / b) * x;
	} else {
		x = 1;
		y = 0;
	}
	return d;
}

/// @brief 任意のmについてn^{-1} mod mを求める.
/// @param n 逆元を求めたい値
/// @param m 法(素数でなくてもよい)
/// @return 存在しなければ-1,存在するなら逆元
ll inv_extgcd(ll n, ll m) {
	ll x, y;
	ll G = extgcd(n, m, x, y);
	if(G != 1) {
		return -1;
	}
	if(x < 0) {
		x += (abs(x) + 2 * m) / m * m;
	}
	x %= m;
	return x;
}