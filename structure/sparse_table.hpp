#include "daylight/base.hpp"

template<typename S, typename F>
struct sparse_table {
private:
	vector<vector<S>> V;
	F op;
	vi lookup;

public:
	/// @brief 前計算を行う
	/// @param _V 配列の値
	/// @param op 演算を表すラムダ式
	sparse_table(vector<S> _V, F op): op(op) {
		int N = SZ(_V);
		int len = 0;
		while((1 << len) <= N) len++;
		V.assign(len, vector<S>(1 << len));
		REP(i, N) {
			V[0][i] = _V[i];
		}
		FOR(i, 1, len) {
			for(int j = 0; j + (1 << i) <= (1 << len);
				j++) {
				V[i][j] = op(V[i - 1][j],
							 V[i - 1][j + (1 << (i - 1))]);
			}
		}
		lookup.resize(N + 1);
		FOR(i, 2, SZ(lookup)) {
			lookup[i] = lookup[i >> 1] + 1;
		}
	}
	/// @brief [l,r)の演算結果を取得する
	/// @param l 区間の左端(inclusive)
	/// @param r 区間の右端(exclusive)
	/// @return 演算結果
	inline S prod(int l, int r) {
		int len = lookup[r - l];
		return op(V[len][l], V[len][r - (1 << len)]);
	}
};