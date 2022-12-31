#pragma once
template<typename T>
struct AbelAdd {
	using value_type = T;
	static constexpr T op(const T& a, const T& b) noexcept {
		return a + b;
	}
	static constexpr T inv(const T& a) noexcept {
		return -a;
	}
	static constexpr T e() {
		return T(0);
	}
};
template<typename T>
struct AbelXor {
	using value_type = T;
	static constexpr T op(const T& a, const T& b) noexcept {
		return a ^ b;
	}
	static constexpr T inv(const T& a) noexcept {
		return a;
	}
	static constexpr T e() {
		return T(0);
	}
};