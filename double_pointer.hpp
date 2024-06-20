#include "daylight/base.hpp"
/// @brief 各lについてcheck(l,r)==trueになる最小のrを求める。
/// @param left 探索範囲の左端(inclusive)
/// @param right 探索範囲の右端(exclusive)
/// @param check ()->現在の範囲が条件を満たすか
/// @param add (インデックス,左からの操作か)
/// @param del (インデックス,左からの操作か)
/// @param update 各lについてcheck(l,r)==trueになる最大のrとともに1度のみ呼ばれる
void doublePointerMinRight(
	int left, int right, function<bool()> check,
	function<void(int, bool)> add,
	function<void(int, bool)> del,
	function<void(int, int)> update) {
	int r = left;
	for(int l = left; l < right; l++) {
		while(r < right && !check()) {
			add(r++, false);
		}
		if(!check()) break;
		update(l, r);
		del(l, true);
	}
}

/// @brief 各lについてcheck(l,r)==trueになる最大のrを求める。
/// @param left 探索範囲の左端(inclusive)
/// @param right 探索範囲の右端(exclusive)
/// @param check ()->現在の範囲が条件を満たすか
/// @param add (インデックス,左からの操作か)
/// @param del (インデックス,左からの操作か)
/// @param update 各lについてcheck(l,r)==trueになる最大のrとともに1度のみ呼ばれる
void doublePointerMaxRight(
	int left, int right, function<bool()> check,
	function<void(int, bool)> add,
	function<void(int, bool)> del,
	function<void(int, int)> update) {
	int r = left;
	for(int l = left; l < right; l++) {
		while(r < right) {
			add(r++, false);
			if(!check()) {
				r--;
				del(r, false);
				break;
			}
		}
		update(l, r);
		if(l == r) {
			r++;
		} else {
			del(l, true);
		}
	}
}