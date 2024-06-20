#include "daylight/base.hpp"

/// @brief 各要素について、その要素を中心とする最長回文の半径をもとめる
/// @param S 対象の配列
/// @return ret[i]:i番目の要素を中心とする最長回文の半径
template<typename T>
vi manacher(vector<T>& S) {
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
/// @brief 各文字について、その文字を中心とする最長回文の半径をもとめる
/// @param S 対象の文字列
/// @return ret[i]:i文字目を中心とする最長回文の半径
vi manacher(string& S) {
	vector<char> v;
	for(auto c: S) {
		v.push_back(c);
	}
	return manacher(v);
}

/// @brief 2N-1個の場所(要素と、要素と要素の間)について、それを中心とする最長回文の半径
/// @param S 対象のvector
/// @return ret[i](長さ2N-1):左からi番目の場所を中心とする最長回文の半径
template<typename T>
vi ext_manacher(vector<T>& S, T split) {
	vector<T> v;
	int N = SZ(S);
	REP(i, N) {
		if(i != 0) v.push_back(split);
		v.push_back(S[i]);
	}
	auto ret = manacher(v);
	REP(i, 2 * N - 1) {
		if(i % 2 == 0) {
			ret[i] = (ret[i] + 1) / 2;
		} else {
			ret[i] = ret[i] / 2;
		}
	}
	return ret;
}
/// @brief 2N-1個の場所(文字と、文字と文字の間)について、それを中心とする最長回文の半径
/// @param S 対象の文字列
/// @return ret[i](長さ2N-1):左からi番目の場所を中心とする最長回文の半径
vi ext_manacher(string& S, char split = '$') {
	vector<char> v;
	for(auto c: S) {
		v.push_back(c);
	}
	return ext_manacher(v, split);
}