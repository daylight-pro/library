#pragma once
#include "daylight/base.hpp"
template<typename T>
struct Range {
private:
	pair<T, T> P;
	pair<bool, bool> bound;
	pair<bool, bool> inclusive;

public:
	Range() {
	}
	Range(string l, string r, bool inl = true,
		  bool inr = false)
		: inclusive({ inl, inr }) {
		assert(l == "-");
		assert(r == "-");
		bound.first = false;
		bound.second = false;
	}
	Range(string l, T r, bool inl = true, bool inr = false)
		: inclusive({ inl, inr }) {
		assert(l == "-");
		bound.first = false;
		bound.second = true;
		P.second = r;
	}
	Range(T l, string r, bool inl = true, bool inr = false)
		: inclusive({ inl, inr }) {
		assert(r == "-");
		bound.first = true;
		bound.second = false;
		P.first = l;
	}
	Range(T l, T r, bool inl = true, bool inr = false)
		: inclusive({ inl, inr }) {
		assert(l <= r);
		bound.first = true;
		bound.second = true;
		P.first = l;
		P.second = r;
	}
	pair<bool, T> getLeft() {
		return { bound.first, P.first };
	}
	pair<bool, T> getRight() {
		return { bound.second, P.second };
	}
	bool isLeftInclusive() {
		return inclusive.first;
	}
	bool isRightInclusive() {
		return inclusive.second;
	}
};