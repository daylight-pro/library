#include "daylight/base.hpp"
#include "daylight/graph/base.hpp"
template<typename Data, typename Cost = ll>
struct Rerooting {
	vector<Data> dp, memo;
	Graph<> G;
	int N = -1;
	using F1 = function<Data(Data, Data)>;
	using F2 = function<Data(Data, Edge<Cost>)>;
	F1 merge;
	F2 apply;
	Data e, leaf;
	/// @brief コンストラクタ
	/// @param n 頂点数
	/// @param merge モノイドの合成関数
	/// @param apply 部分木の寄与を求める関数
	/// @param e モノイドの単位元
	/// @param leaf 葉のモノイド
	Rerooting(int n, F1 merge, F2 apply, Data e, Data leaf)
		: N(n),
		  merge(merge),
		  apply(apply),
		  e(e),
		  leaf(leaf) {
		G = Graph<Cost>(n);
	}

	/// @brief 辺を追加する
	/// @param e 追加する辺
	void add_edge(Edge<Cost> e) {
		assert(0 <= e.from && e.from < N && 0 <= e.to
			   && e.to < N);
		G[e.from].push_back(e);
	}

	/// @brief 全方位木DPを行う
	/// @return 全方位木DPの結果
	vector<Data> build() {
		memo = vector<Data>(SZ(G), e);
		dp = vector<Data>(SZ(G));
		dfs1(0);
		dfs2(0, e);
		return dp;
	}

private:
	void dfs1(int cur, int pre = -1) {
		bool isLeaf = true;
		for(Edge<Cost> edge: G[cur]) {
			if(edge.to == pre) continue;
			dfs1(edge.to, cur);
			isLeaf = false;
			memo[cur] = merge(
				memo[cur],
				apply(memo[edge.to],
					  Edge(edge.to, cur, edge.cost)));
		}
		if(isLeaf) memo[cur] = leaf;
	}

	void dfs2(int cur, const Data val, int pre = -1) {
		// dsはcurから行くことができる全ての頂点について
		// その頂点を根とした部分木の寄与を配列にしたもの
		// 一番最初が親からの寄与、そのあとに子からの寄与
		vector<Data> ds { val };
		for(Edge<Cost> edge: G[cur]) {
			if(edge.to == pre) continue;
			ds.push_back(
				apply(memo[edge.to],
					  Edge<Cost>(edge.to, cur, edge.cost)));
		}
		int N = SZ(ds);
		vector<Data> dp_left(N + 1, e), dp_right(N + 1, e);
		// dp_left[i] => dsの[0,i)の寄与の総和
		REP(i, N) dp_left[i + 1] = merge(dp_left[i], ds[i]);
		// dp_right[i] => dsの[i,N)の寄与の総和
		REPR(i, N)
		dp_right[i] = merge(dp_right[i + 1], ds[i]);
		// curを根とする場合の答えはdsの[0,N)の寄与の総和
		dp[cur] = dp_left[N];
		int ind = 1;  // 親以外の頂点にインデックスをつける
		for(Edge<Cost> edge: G[cur]) {
			if(edge.to == pre) continue;
			// edge.to以外のcurにつながる頂点を頂点とした部分木の寄与の総和
			Data sub
				= merge(dp_left[ind], dp_right[ind + 1]);
			dfs2(edge.to,
				 apply(sub,
					   Edge<Cost>(cur, edge.to, edge.cost)),
				 cur);
			ind++;
		}
	}
};