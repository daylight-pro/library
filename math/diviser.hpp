#include "daylight/base.hpp"
/// @brief 約数を全列挙する
/// @param N 整数N
/// @return Nの約数が小さい順に格納されたリスト
vll get_divisers(ll N) {
	vll ret;
	for(ll i = 1; i * i <= N; i++) {
		if(N % i == 0) {
			ret.push_back(i);
			if(i * i != N) ret.push_back(N / i);
		}
	}
	so(ret);
	return ret;
}