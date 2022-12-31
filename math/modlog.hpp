#include <atcoder/all>
#include "daylight/base.hpp"
using namespace atcoder;

/// @brief a^n=b(mod m)となる最小のnを求める
/// @param a パラメータ
/// @param b パラメータ
/// @param m 除数
/// @return nが存在しない場合-1,存在する場合n
ll modlog(ll a, ll b, ll m) {
	a %= m;
	b %= m;
	if(a == 0) {
		if(b == 1)
			return 0;
		else if(b == 0)
			return 1;
		else
			return -1;
	}
	if(b == 1) {
		return 0;
	}
	ll sq = 1;
	while(sq * sq < m) sq++;
	map<ll, ll> A;
	ll cur = a;
	FOR(r, 1, sq) {
		if(!A.count(cur)) A[cur] = r;
		cur *= a;
		cur %= m;
	}
	using mint = modint;
	mint::set_mod(m);
	ll a_powM = mint(a).inv().pow(sq).val();
	cur = b;
	REP(q, sq) {
		if(cur == 1 && q > 0) return q * sq;
		if(A.count(cur)) return q * sq + A[cur];
		cur *= a_powM;
		cur %= m;
	}
	return -1;
}