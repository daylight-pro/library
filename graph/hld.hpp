class HLdecomposition {
private:
	int V;
	vector<vector<int>> G;
	vector<int> stsize, parent, pathtop, in, out;
	int root;
	bool built_flag;
	void BuildStsize(int u, int p) {
		stsize[u] = 1, parent[u] = p;
		for(int& v: G[u]) {
			if(v == p) {
				if(v == G[u].back())
					break;
				else
					swap(v, G[u].back());
			}
			BuildStsize(v, u);
			stsize[u] += stsize[v];
			if(stsize[v] > stsize[G[u][0]]) {
				swap(v, G[u][0]);
			}
		}
	}
	void BuildPath(int u, int p, int& tm) {
		in[u] = tm++;
		for(int v: G[u]) {
			if(v == p) continue;
			pathtop[v] = (v == G[u][0] ? pathtop[u] : v);
			BuildPath(v, u, tm);
		}
		out[u] = tm;
	}

public:
	void add_edge(int u, int v) {
		G[u].push_back(v), G[v].push_back(u);
	}
	void build(int _root = 0) {
		root = _root;
		built_flag = true;
		int tm = 0;
		BuildStsize(root, -1);
		pathtop[root] = root;
		BuildPath(root, -1, tm);
	}
	//元の頂点のインデックスの配列上でのidを返す
	inline int get(int a) {
		return in[a];
	}
	int lca(int a, int b) {
		int pa = pathtop[a], pb = pathtop[b];
		while(pathtop[a] != pathtop[b]) {
			if(in[pa] > in[pb]) {
				a = parent[pa], pa = pathtop[a];
			} else {
				b = parent[pb], pb = pathtop[b];
			}
		}
		if(in[a] > in[b]) swap(a, b);
		return a;
	}
	PI subtree_query(int a) {
		assert(
			built_flag
			&& "You must call hld.build() before call query function");
		return { in[a], out[a] };
	}
	vector<tuple<int, int, bool>> path_query(int from,
											 int to) {
		assert(
			built_flag
			&& "You must call hld.build() before call query function");
		int pf = pathtop[from], pt = pathtop[to];
		using T = tuple<int, int, bool>;
		deque<T> front, back;
		while(pathtop[from] != pathtop[to]) {
			if(in[pf] > in[pt]) {
				front.emplace_back(in[pf], in[from] + 1,
								   true);
				from = parent[pf], pf = pathtop[from];
			} else {
				back.emplace_front(in[pt], in[to] + 1,
								   false);
				to = parent[pt], pt = pathtop[to];
			}
		}
		if(in[from] > in[to]) {
			front.emplace_back(in[to], in[from] + 1, true);
		} else {
			front.emplace_back(in[from], in[to] + 1, false);
		}
		vector<T> ret;
		while(!front.empty()) {
			ret.push_back(front.front());
			front.pop_front();
		}
		while(!back.empty()) {
			ret.push_back(back.front());
			back.pop_front();
		}
		return ret;
	}
	HLdecomposition(int node_size)
		: V(node_size),
		  G(V),
		  stsize(V, 0),
		  parent(V, -1),
		  pathtop(V, -1),
		  in(V, -1),
		  out(V, -1),
		  built_flag(false) {
	}
};