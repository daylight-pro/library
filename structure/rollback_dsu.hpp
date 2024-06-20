class rollback_dsu {
public:
	int N;
	vi p;
	stack<PI> history;
	dsu() {
	}
	dsu(int size): N(size) {
		p.resize(N, -1);
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
		if(p[x] > p[y]) swap(x, y);
		if(p[x] == p[y]) {
			history.push({ x, p[x] });
			p[x]--;
		}
		history.push({ y, p[y] });
		p[y] = x;
	}
	//xが属している木の根を探る
	int leader(int x) {
		if(p[x] < 0)
			return x;
		else
			return leader(p[x]);
	}
	void snapshot() {
		history.clear();
	}
	void rollback() {
		while(!history.empty()) {
			auto [i, v] = history.top();
			history.pop();
			p[i] = v;
		}
	}
};