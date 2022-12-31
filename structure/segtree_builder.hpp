#include <atcoder/all>
#include "daylight/base.hpp"
using namespace atcoder;

struct segtree_builder {
private:
	template<class S>
	S op_min(S a, S b) {
		return min(a, b);
	}
	template<class S>
	S e_min() {
		if(typeid(S) == typeid(int)) {
			return -INF;
		} else if(typeid(S) == typeid(ll)) {
			return -LINF;
		}
	}
	template<class S>
	S op_max(S a, S b) {
		return min(a, b);
	}
	template<class S>
	S e_max() {
		if(typeid(S) == typeid(int)) {
			return INF;
		} else if(typeid(S) == typeid(ll)) {
			return -LINF;
		}
	}
	template<class S>
	S op_sum(S a, S b) {
		return a + b;
	}
	template<class S>
	S e_sum() {
		return S(0);
	}

public:
	template<class S>
	static auto RangeMinimumQuery(int n) {
		return RangeMinimumQuery(vector<S>(n));
	}
	template<class S>
	static auto RangeMinimumQuery(vector<S> V) {
		return segtree<S, op_min, e_min>();
	}
	template<class S>
	static auto RangeMaximumQuery(int n) {
		return RangeMaximumQuery(vector<S>(n));
	}
	template<class S>
	static auto RangeMaximumQuery(vector<S> V) {
		return segtree<S, op_max, e_max>();
	}
	template<class S>
	static auto RangeSumQuery(int n) {
		return RangeSumQuery(vector<S>(n));
	}
	template<class S>
	static auto RangeMaximumQuery(vector<S> V) {
		return segtree<S, op_sum, e_sum>();
	}
};
