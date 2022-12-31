#include "daylight/base.hpp"
template<typename S = __int128_t, bool doReduction = true>
struct Rational {
private:
	S child, parent;
	S originalChild, originalParent;
	using R = Rational<S, doReduction>;

public:
	Rational() {
		R(0, 1);
	}
	/// @brief コンストラクタ
	/// @param c 分子
	/// @param p 分母
	Rational(S c, S p) {
		assert(p != 0);
		originalChild = c;
		originalParent = p;
		if(c == 0) {
			child = 0;
			parent = 1;
		} else if(!doReduction) {
			child = c;
			parent = p;
		} else {
			S g = gcd(abs(c), abs(p));
			c /= g;
			p /= g;
			c *= p / abs(p);
			child = c;
			parent = abs(p);
		}
	}
	R operator+(const R& r) const {
		S new_p = parent * r.parent;
		S new_c = child * new_p / parent
			+ r.child * new_p / r.parent;
		return R(new_c, new_p);
	}
	R operator-(const R& r) const {
		S new_p = parent * r.parent;
		S new_c = child * new_p / parent
			- r.child * new_p / r.parent;
		return R(new_c, new_p);
	}
	R operator*(const R& r) const {
		return R(child * r.child, parent * r.parent);
	}
	R operator/(const R& r) const {
		return R(child * r.parent, parent * r.child);
	}
	bool operator==(const R& r) const {
		return child == r.child && parent == r.parent;
	}
	bool operator!=(const R& r) const {
		return child != r.child || parent != r.parent;
	}
	bool operator<(const R& r) const {
		return child * r.parent < parent * r.child;
	}
	bool operator>(const R& r) const {
		return child * r.parent > parent * r.child;
	}
	bool operator<=(const R& r) const {
		return !(*this > r);
	}
	bool operator>=(const R& r) const {
		return !(*this < r);
	}
	R inv() const {
		return R(parent, child);
	}
	ll getChild() const {
		return ll(child);
	}
	ll getParent() const {
		return ll(parent);
	}
	ll getOriginalChild() const {
		return originalChild;
	}
	ll getOriginalParent() const {
		return originalParent;
	}
	/// @brief 分数を少数に変換する
	/// @return 分数の表す少数
	double getDouble() {
		return double(child) / parent;
	}
};