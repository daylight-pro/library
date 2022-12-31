#include "daylight/base.hpp"
template<typename T>
vector<vector<T>> multiMatrix(const vector<vector<T>> &A,
							  const vector<vector<T>> &B) {
	int N = SZ(A);
	int M = SZ(B[0]);
	int L = SZ(A[0]);
	auto ret = make_v<T>(N, M);
	REP(i, N) {
		REP(j, M) {
			REP(k, L) {
				ret[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return ret;
}

template<typename T>
vector<vector<T>> powMatrix(const vector<vector<T>> &A,
							ll p) {
	auto ans = make_v<T>(SZ(A), SZ(A));
	REP(i, SZ(A)) ans[i][i] = 1;
	auto mul = A;
	for(; p > 0; p >>= 1) {
		if((p & 1) == 1) ans = multiMatrix(ans, mul);
		mul = multiMatrix(mul, mul);
	}
	return ans;
}