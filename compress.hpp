#pragma once
#include "daylight/base.hpp"
template<typename T>
struct Compress {
private:
	map<T, int> table;
	vector<T> V;
	bool has_built;

public:
	Compress(): has_built(false) {
	}

	void add(const T x) {
		assert(!has_built);
		V.push_back(x);
	}
	void add(const vector<T> &x) {
		assert(!has_built);
		for(auto v: x) {
			V.push_back(v);
		}
	}
	void build() {
		int ind = 0;
		so(V);
		uni(V);
		for(auto v: V) {
			table[v] = ind;
			ind++;
		}
		has_built = true;
	}
	int get(T x) {
		assert(has_built);
		return table[x];
	}
	T operator[](int i) {
		return V[i];
	}
	size_t size() {
		return V.size();
	}
};