#pragma once
#include "daylight/base.hpp"
ll powmod(ll a, ll p, ll m = MOD) {
	ll ans = 1;
	ll mul = a;
	for(; p > 0; p >>= 1) {
		if(p & 1) ans = (ans * mul) % m;
		//mulを更新。2回分の操作を入れる。
		mul = (mul * mul) % m;
	}
	return ans;
}