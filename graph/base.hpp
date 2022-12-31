#pragma once
#include "daylight/base.hpp"
template<class T = ll>
struct Edge {
public:
	int from, to;
	T cost;
	Edge() {
	}
	Edge(int _from, int _to, T _cost) {
		from = _from;
		to = _to;
		cost = _cost;
	}
};
template<class T = ll>
using Edges = vector<Edge<T>>;
template<class T = ll>
using Graph = vector<Edges<T>>;