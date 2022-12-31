#include "daylight/base.hpp"

/// @brief 各文字について、その文字を中心とする最長回文の半径をもとめる
/// @param S 対象の文字列
/// @return ret[i]:i文字目を中心とする最長回文の半径
vi manacher(string& S) {
	int N = SZ(S);
	int i = 0;
	int j = 0;
	vi ret(N);
	while(i < N) {
		while(i - j >= 0 && i + j < N
			  && S[i - j] == S[i + j])
			j++;
		ret[i] = j;
		int k = 1;
		while(i - k >= 0 && k + ret[i - k] < j) {
			ret[i + k] = ret[i - k];
			k++;
		}
		i += k;
		j -= k;
	}
	return ret;
}

/// @brief 2N-1個の場所(文字と、文字と文字の間)について、それを中心とする最長回文の半径
/// @param S 対象の文字列
/// @return ret[i](長さ2N-1):左からi番目の場所を中心とする最長回文の半径
vi ext_manacher(string& S) {
	stringstream ss;
	int N = SZ(S);
	REP(i, N) {
		if(i != 0) ss << "$";
		ss << S[i];
	}
	string s = ss.str();
	auto ret = manacher(s);
	REP(i, 2 * N - 1) {
		if(i % 2 == 0) {
			ret[i] = (ret[i] + 1) / 2;
		} else {
			ret[i] = ret[i] / 2;
		}
	}
	return ret;
}