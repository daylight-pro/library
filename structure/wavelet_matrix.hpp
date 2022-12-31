#include "daylight/base.hpp"
struct BitVector {
private:
	vector<int> vec;
	int len;

public:
	BitVector() {
	}
	BitVector(const vector<int>& v) {
		len = SZ(v);
		vec.resize(len + 1, 0);
		REP(i, len) {
			vec[i + 1] += vec[i];
			vec[i + 1] += v[i];
		}
	}
	// i ビット目を取得
	int get(const unsigned int i) {
		return vec[i + 1] - vec[i];
	}

	// [0, i) ビットの 1 の数
	unsigned int rank1(const unsigned int i) const {
		return vec[i];
	}
	// [i, j) ビットの 1 の数
	unsigned int rank1(const unsigned int i,
					   const unsigned int j) const {
		return rank1(j) - rank1(i);
	}
	// [0, i) ビットの 0 の数
	unsigned int rank0(const unsigned int i) const {
		return i - rank1(i);
	}
	// [i, j) ビットの 0 の数
	unsigned int rank0(const unsigned int i,
					   const unsigned int j) const {
		return rank0(j) - rank0(i);
	}

	// [0, i) ビットの b の数
	unsigned int rank(const unsigned int b,
					  const unsigned int i) const {
		if(b == 0)
			return rank0(i);
		else
			return rank1(i);
	}

	// k+1番目のbのインデックス
	unsigned int select(const unsigned int b,
						const unsigned int k) const {
		if(rank(b, len) >= k) return -1;
		int ok = 0;
		int ng = len;
		while(abs(ng - ok) > 1) {
			int mid = (ng + ok) / 2;
			if(rank(b, mid) <= k)
				ok = mid;
			else
				ng = mid;
		}
		return ok;
	}
};
template<typename T>
struct WaveletMatrix {
private:
	vector<BitVector> B;  //ビットベクトル
	vector<vector<T>> acc;	//各行での累積和
	vi start_one;  //ソート結果の1の開始地点
	map<T, int> start_num;	//最終結果の各数字の開始地点
	int len, bit_len;

public:
	WaveletMatrix(const vector<T>& vec,
				  bool use_acc = true) {
		ll max_el
			= vec.empty() ? 1 : *max_element(ALL(vec)) + 1;
		bit_len = (max_el == 1)
			? 1
			: (64 - __builtin_clzll(max_el - 1));
		len = SZ(vec);
		if(use_acc)
			acc = vector(bit_len, vector<T>(len + 1));
		start_one = vi(bit_len);
		vector<T> v(vec);
		REP(b, bit_len) {
			vector<T> cur;
			vi bi(len);
			REP(i, len) {
				ll bit = (v[i] >> T(bit_len - b - 1)) & 1;
				if(bit == 0) {
					cur.push_back(v[i]);
					bi[i] = 0;
				}
			}
			start_one[b] = SZ(cur);
			REP(i, len) {
				ll bit = (v[i] >> T(bit_len - b - 1)) & 1;
				if(bit == 1) {
					cur.push_back(v[i]);
					bi[i] = 1;
				}
			}
			B.push_back(BitVector(bi));
			if(use_acc) {
				REP(i, len) {
					if(B[b].get(i) == 0)
						acc[b][i + 1] = v[i];
					acc[b][i + 1] += acc[b][i];
				}
			}
			v = cur;
		}

		REPR(i, len) {
			start_num[v[i]] = i;
		}
	}

	/// @brief v[i]にアクセスする
	/// @param i アクセスするインデックス
	/// @return v[i]の値
	T access(int i) {
		assert(i < len);

		T ret = 0;
		REP(j, bit_len) {
			int b = B[j].get(i);
			ret <<= 1;
			ret |= b;
			i = B[j].rank(b, i);
			if(b == 1) {
				i += start_one[j];
			}
		}
		return ret;
	}

	/// @brief 区間[0,k)の中に、cがいくつ含まれるか求める
	/// @param c 検索対象の値
	/// @param k 区間の右端(exclusive)
	/// @return [0,k)でのcの出現回数
	int rank(T c, int k) {
		assert(k <= len);
		assert(k >= 0);
		if(start_num.find(c) == start_num.end()) return 0;
		REP(i, bit_len) {
			ll bit = (c >> T(bit_len - i - 1)) & 1;
			k = B[i].rank(bit, k);
			if(bit == 1) {
				k += start_one[i];
			}
		}
		return k - start_num[c];
	}

	/// @brief 区間[left,right)でk番目に小さい値を求める
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param k 検索対象の順位(0-indexed)
	/// @return [left,right)でk番目に小さい値
	T kthMin(int left, int right, int k) {
		assert(right - left > k);
		assert(left < right);
		ll res = 0;
		REP(i, bit_len) {
			ll left_rank = B[i].rank(0, left);
			ll right_rank = B[i].rank(0, right);
			ll zero_in_range = right_rank - left_rank;
			ll bit = (k < zero_in_range) ? 0 : 1;
			if(bit == 1) {
				k -= zero_in_range;
				left += start_one[i] - left_rank;
				right += start_one[i] - right_rank;
			} else {
				left = left_rank;
				right = right_rank;
			}
			res <<= 1;
			res |= bit;
		}
		return res;
	}

	/// @brief 区間[left,right)でk番目に大きい値を求める
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param k 検索対象の順位(0-indexed)
	/// @return [left,right)でk番目に大きい値
	T kthMax(int left, int right, int k) {
		assert(right - left > k);
		assert(left < right);
		int len = right - left;
		int nk = len - k - 1;
		return kthMin(left, right, nk);
	}

	/// @brief [left,right)の要素を昇順に並べた先頭k要素の和を求める
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param k 先頭要素の個数
	/// @return 昇順先頭k要素の和
	T kMinSum(int left, int right, int k) {
		assert(right - left >= k);
		assert(left < right);
		ll kth = 0;
		ll ret = 0;
		REP(i, bit_len) {
			ll left_rank = B[i].rank(0, left);
			ll right_rank = B[i].rank(0, right);
			ll zero_in_range = right_rank - left_rank;
			ll bit = (k < zero_in_range) ? 0 : 1;
			if(bit == 1) {
				ret += acc[i][right] - acc[i][left];
				k -= zero_in_range;
				left += start_one[i] - left_rank;
				right += start_one[i] - right_rank;
			} else {
				left = left_rank;
				right = right_rank;
			}
			kth <<= 1;
			kth |= bit;
		}
		ret += kth * k;
		return ret;
	}
	/// @brief [left,right)の要素を降順に並べた先頭k要素の和を求める
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param k 先頭要素の個数
	/// @return 降順先頭k要素の和
	T kMaxSum(int left, int right, int k) {
		assert(right - left >= k);
		assert(left < right);
		return kMinSum(left, right, right - left)
			- kMinSum(left, right, right - left - k);
	}
	/// @brief [l,r)でupper未満の値の数を数える
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param upper 検索対象の値(exclusive)
	/// @return upper未満の数の値
	int lessCount(int left, int right, T upper) {
		assert(left <= right);
		ll ret = 0;
		if(left == right) {
			return 0;
		}
		if(upper >= T(T(1) << T(bit_len))) {
			return right - left;
		}
		REP(i, bit_len) {
			ll left_rank = B[i].rank(0, left);
			ll right_rank = B[i].rank(0, right);
			ll zero_in_range = right_rank - left_rank;
			ll bit = (upper >> T(bit_len - i - 1)) & 1;
			if(bit == 1) {
				ret += zero_in_range;
				left += start_one[i] - left_rank;
				right += start_one[i] - right_rank;
			} else {
				left = left_rank;
				right = right_rank;
			}
		}
		return ret;
	}
	/// @brief [l,r)でupper以下の値の数を数える
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param upper 検索対象の値(inclusive)
	/// @return upper以下の数の値
	int lessEqualCount(int left, int right, T upper) {
		assert(left <= right);
		return lessCount(left, right, upper)
			+ rangeFreq(left, right, upper);
	}
	/// @brief [l,r)でlower超の値の数を数える
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索対象の値(exclusive)
	/// @return lower超の数の値
	int greaterCount(int left, int right, T lower) {
		assert(left <= right);
		return right - left
			- lessEqualCount(left, right, lower);
	}
	/// @brief [l,r)でlower以上の値の数を数える
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索対象の値(inclusive)
	/// @return lower以上の数の値
	int greaterEqualCount(int left, int right, T lower) {
		assert(left <= right);
		return right - left - lessCount(left, right, lower);
	}
	/// @brief [left,right)の範囲で,値がlower以上の要素の中でk番目に小さい要素を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索する値の下限(inclusive)
	/// @param k 検索対象の順位(0-indexed)
	/// @return k番目の要素の値
	T greaterEqualKthMin(int left, int right, T lower,
						 int k) {
		assert(left < right);
		int cnt = lessCount(left, right, lower);
		return kthMin(left, right, k + cnt);
	}
	/// @brief [left,right)の範囲で,値がlower超の要素の中でk番目に小さい要素を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索する値の下限(exclusive)
	/// @param k 検索対象の順位(0-indexed)
	/// @return k番目の要素の値
	T greaterKthMin(int left, int right, T lower, int k) {
		assert(left < right);
		int cnt = lessEqualCount(left, right, lower);
		return kthMin(left, right, k + cnt);
	}
	/// @brief [left,right)の範囲で,値がupper未満の要素の中でk番目に大きい要素を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param upper 検索する値の上限(exclusive)
	/// @param k 検索対象の順位(0-indexed)
	/// @return k番目の要素の値
	T lessKthMax(int left, int right, T upper, int k) {
		assert(left < right);
		int cnt = lessCount(left, right, upper);
		return kthMin(left, right, cnt - k - 1);
	}
	/// @brief [left,right)の範囲で,値がupper以下の要素の中でk番目に大きい要素を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param upper 検索する値の上限(inclusive)
	/// @param k 検索対象の順位(0-indexed)
	/// @return k番目の要素の値
	T lessEqualKthMax(int left, int right, T upper, int k) {
		assert(left < right);
		int cnt = lessEqualCount(left, right, upper);
		return kthMin(left, right, cnt - k - 1);
	}
	/// @brief [left,right)の範囲で,値がupper未満の要素の中で降順先頭k要素の和を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param upper 検索する値の上限(exclusive)
	/// @param k 先頭要素の個数
	/// @return 降順先頭k要素の和
	T lessKMaxSum(int left, int right, T upper, int k) {
		assert(left < right);
		int cnt = greaterEqualCount(left, right, upper);
		return kMaxSum(left, right, cnt + k)
			- kMaxSum(left, right, cnt);
	}
	/// @brief [left,right)の範囲で,値がupper以下の要素の中で降順先頭k要素の和を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param upper 検索する値の上限(inclusive)
	/// @param k 先頭要素の個数
	/// @return 降順先頭k要素の和
	T lessEqualKMaxSum(int left, int right, T upper,
					   int k) {
		assert(left < right);
		int cnt = greaterCount(left, right, upper);
		return kMaxSum(left, right, cnt + k)
			- kMaxSum(left, right, cnt);
	}
	/// @brief [left,right)の範囲で,値がlower超の要素の中で昇順先頭k要素の和を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索する値の下限(exclusive)
	/// @param k 先頭要素の個数
	/// @return 昇順先頭k要素の和
	T greaterKMinSum(int left, int right, T lower, int k) {
		assert(left < right);
		int cnt = lessEqualCount(left, right, lower);
		return kMinSum(left, right, cnt + k)
			- kMinSum(left, right, cnt);
	}
	/// @brief [left,right)の範囲で,値がlower以上の要素の中で昇順先頭k要素の和を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索する値の下限(inclusive)
	/// @param k 先頭要素の個数
	/// @return 昇順先頭k要素の和
	T greaterEqualKMinSum(int left, int right, T lower,
						  int k) {
		assert(left < right);
		int cnt = lessCount(left, right, lower);
		return kMinSum(left, right, cnt + k)
			- kMinSum(left, right, cnt);
	}
	/// @brief [left,right)の範囲で,値が[lower,upper)の範囲にある要素の数を数える
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索する値の下限(inclusive)
	/// @param upper 検索する値の上限(exclusive)
	/// @return 該当する要素の個数
	int valueRangeCount(int left, int right, T lower,
						T upper) {
		assert(left <= right);
		return lessCount(left, right, upper)
			- lessCount(left, right, lower);
	}
	/// @brief [left,right)の範囲で,値が[lower,upper)の範囲にある要素の和を求める.
	/// @param left 検索対象の左端(inclusive)
	/// @param right 検索対象の右端(exclusive)
	/// @param lower 検索する値の下限(inclusive)
	/// @param upper 検索する値の上限(exclusive)
	/// @return 検索対象の和
	T valueRangeSum(int left, int right, T lower, T upper) {
		assert(left <= right);
		int less = lessCount(left, right, lower);
		int greater = greaterEqualCount(left, right, upper);
		return kMaxSum(left, right, right - left)
			- kMaxSum(left, right, greater)
			- kMinSum(left, right, less);
	}
	/// @brief 区間[l,r)でvが何回出現するか求める
	/// @param l 検索対象の左端(inclusive)
	/// @param r 検索対象の右端(exclusive)
	/// @param v 検索対象の値
	/// @return [l,r)でのvの出現回数
	int rangeFreq(int l, int r, T v) {
		assert(0 <= l && l <= r && r <= len);
		return rank(v, r) - rank(v, l);
	}
};