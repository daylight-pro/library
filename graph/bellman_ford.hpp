#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
/// @brief ベルマンフォード法で単一始点最短経路長問題を解く
/// @param V 頂点数
/// @param EG 辺のリスト
/// @param s 始点
/// @param D out:最短経路長が入る配列
/// @return 負閉路を含むかどうか
bool shortest_path(int V, Edges<ll>& EG, int s, vll& D) {
	D = vll(V, LINF);
	D[s] = 0;
	int cnt = 0;
	while(true) {
		bool update = false;  //このループで更新が起こったか
		for(auto e: EG) {
			if(D[e.from] == LINF) continue;
			if(chmin(D[e.to], D[e.from] + e.cost)) {
				//このEdgeを通ると暫定より低コストでe.toに行ける場合
				D[e.to] = D[e.from] + e.cost;  //値を更新
				update = true;
			}
		}
		if(!update) return false;  //最短距離の更新が終了
		cnt++;
		if(cnt == V) return true;
	}
}