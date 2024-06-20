class dsu {
public:
	int treeCnt;  //ツリーの数
	vi p;  //p => 親
	vi num;	 //木の要素数
	vi rank;
	int N;
	dsu() {
	}
	dsu(int size) {
		N = size;
		p.resize(N, 0);
		num.resize(N, 1);
		rank.resize(N, 0);
		treeCnt = N;
		REP(i, N) {
			p[i] = i;
		}
	}

	//同じ木に属しているか判定
	bool same(int x, int y) {
		return leader(x) == leader(y);
	}
	//xとyが属している木をつなぐ
	void merge(int x, int y) {
		link(leader(x), leader(y));
	}
	//木と木を繋ぐ処理をする。
	void link(int x, int y) {
		if(x == y) return;
		if(rank[x] < rank[y]) swap(x, y);
		p[y] = x;
		num[x] += num[y];
		if(rank[x] == rank[y]) rank[x]++;
		treeCnt--;
	}
	//xが属している木の根を探る
	int leader(int x) {
		if(p[x] == x)
			return x;
		else
			return p[x] = leader(p[x]);
	}
	//このUnion-Find木に存在するグループの数を返す。
	int countTree() {
		return treeCnt;
	}
	//aが所属するグループの要素数を返す。
	int size(int a) {
		return num[leader(a)];
	}
	map<int, vi> groups() {
		map<int, vi> ret;
		REP(i, N) {
			ret[leader(i)].push_back(i);
		}
		return ret;
	}
};