#include "daylight/base.hpp"
/// @brief 全点対最短経路長問題を解く
/// @param n 頂点数
/// @param WF 隣接行列(out:最短経路長)
void floyd(int n, vector<vll>& WF) {
	REP(k, n)
	REP(i, n) {
		if(WF[i][k] == LINF) continue;
		REP(j, n) {
			if(WF[k][j] == LINF) continue;
			chmin(WF[i][j], WF[i][k] + WF[k][j]);
		}
	}
}