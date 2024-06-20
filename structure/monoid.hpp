#include "daylight/base.hpp"

struct MinMonoid {
	using value_type = ll;
	static const bool isOperator = false;
	static constexpr ll e() {
		return LINF;
	}
	static ll op(ll a, ll b) {
		return min(a, b);
	}
};
template<class T>
struct SumMonoid {
	using value_type = T;
	static const bool isOperator = false;
	static constexpr T e() {
		return T(0);
	}
	static T op(T a, T b) {
		return a + b;
	}
};
template<class T>
struct AddMonoid {
	using value_type = T;
	static const bool isOperator = true;
	static constexpr T e() {
		return T(0);
	}
	static T op(T a, T b) {
		return a + b;
	}
};
struct UpdateMonoid {
	using value_type = ll;
	static const bool isOperator = true;
	static constexpr ll e() {
		return 4e18;
	}
	static ll op(ll a, ll b) {
		return b;
	}
};

template<class T>
struct LinerFunctionMonoid {
	using value_type = T;
	static const bool isOperator = true;
	static constexpr T e() {
		return make_pair(1, 0);
	}
	static T op(T a, T b) {
		return make_pair(a.first * b.first,
						 b.first * a.second + b.second);
	}
};