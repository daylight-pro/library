#include "daylight/base.hpp"
#include "daylight/structure/abel.hpp"
template<typename Abel = AbelAdd<ll>>
class potential_uf {
public:
	using T = typename Abel::value_type;
	int treeCnt;  //ツリーの数
	vi p;  //p => 親
	vi num;	 //木の要素数
	vi rank;
	vector<T> potential;
	int N;
	potential_uf() {
	}
	potential_uf(int size) {
		N = size;
		p.resize(N, 0);
		num.resize(N, 1);
		rank.resize(N, 0);
		potential.resize(N, Abel::e());
		treeCnt = N;
		REP(i, N) {
			p[i] = i;
		}
	}

	/// @brief 頂点xと頂点yが連結か判定する
	/// @param x 一つ目の頂点(0-indexed)
	/// @param y 二つ目の頂点(0-indexed)
	/// @return 連結ならtrue
	bool same(int x, int y) {
		return leader(x) == leader(y);
	}

	/// @brief w[y] - w[x] = wとなるようにxとyを接続する
	/// @param x 親の頂点
	/// @param y 子の頂点
	/// @param w ポテンシャルの差
	void merge(int x, int y, T w) {
		w = Abel::op(w, get_potential(x));
		w = Abel::op(w, Abel::inv(get_potential(y)));
		link(leader(x), leader(y), w);
	}

private:
	void link(int x, int y, T w) {
		if(x == y) return;
		if(rank[x] < rank[y]) swap(x, y), w = Abel::inv(w);
		// xが新たな親になるようにyを接続
		p[y] = x;
		num[x] += num[y];
		if(rank[x] == rank[y]) rank[x]++;
		treeCnt--;
		potential[y] = w;
	}

public:
	/// @brief xが属している木の根を取得
	/// @param x 頂点番号(0-indexed)
	/// @return xが属している木の根の頂点番号
	int leader(int x) {
		if(p[x] == x)
			return x;
		else {
			int l = leader(p[x]);
			potential[x]
				= Abel::op(potential[x], potential[p[x]]);
			return p[x] = l;
		}
	}

	/// @brief 頂点xの根からのポテンシャルを求める
	/// @param x 頂点番号(0-indexed)
	/// @return 頂点xの根からの重み
	int get_potential(int x) {
		leader(x);
		return potential[x];
	}

	/// @brief w[y]-w[x]を返す。
	/// @param x 親の頂点(0-indexed)
	/// @param y 子の頂点(0-indexed)
	/// @return 重みの差
	int diff(int x, int y) {
		return Abel::op(get_potential(y),
						Abel::inv(get_potential(x)));
	}

	/// @brief 連結成分の個数を返す
	/// @return 連結成分の個数
	int countTree() {
		return treeCnt;
	}

	/// @brief 頂点aが属する連結成分の要素数を求める
	/// @param a 頂点番号(0-indexed)
	/// @return 頂点aと同じ連結成分に属する要素数
	int size(int a) {
		return num[leader(a)];
	}

	vvi groups() {
		map<int, vi> m;
		REP(i, N) {
			m[leader(i)].push_back(i);
		}
		vvi ret;
		for(auto [_, v]: m) {
			ret.push_back(v);
		}
		return ret;
	}
};