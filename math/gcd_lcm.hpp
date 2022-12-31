#include "daylight/base.hpp"
ll gcd(ll x, ll y) {
	ll r = 0;
	if(x < y) swap(x, y);
	while(y > 0) {
		r = x % y;
		x = y;
		y = r;
	}
	return x;
}

ll lcm(ll a, ll b) {
	return a / gcd(a, b) * b;
}