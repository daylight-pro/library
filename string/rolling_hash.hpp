#include "daylight/base.hpp"
struct RollingHash {
private:
	static const uint64_t mod = (1ull << 61ull) - 1;
	using uint128_t = __uint128_t;
	uint64_t base1, base2;
	vector<uint64_t> pow1, pow2;

	// 必要分のpow配列を追加で求める。
	inline void expand(int sz) {
		int pre_sz = SZ(pow1);
		if(pre_sz < sz + 1) {
			for(int i = pre_sz - 1; i < sz; i++) {
				pow1.push_back(mul(pow1[i], base1));
				pow2.push_back(mul(pow2[i], base2));
			}
		}
	}
	static inline uint64_t add(uint64_t a, uint64_t b) {
		if((a += b) >= mod) a -= mod;
		return a;
	}

	static inline uint64_t mul(uint64_t a, uint64_t b) {
		uint128_t c = (uint128_t) a * b;
		return add(c >> 61, c & mod);
	}

public:
	/// @brief 初期化。基底の選定を行う
	RollingHash() {
		mt19937_64 mt(chrono::steady_clock::now()
						  .time_since_epoch()
						  .count());
		uniform_int_distribution<uint64_t> rand(
			1e9, RollingHash::mod - 1);
		base1 = rand(mt);
		base2 = rand(mt);
		pow1.push_back(1);
		pow2.push_back(1);
	}

	/// @brief 与えられた文字列について,hash値を計算する
	/// @param s hash値を求める文字列
	/// @return 計算された2通りのhash値
	pair<vector<uint64_t>, vector<uint64_t>> build(
		const string& s) {
		expand(SZ(s) + 1);
		vector<uint64_t> hash1
			= vector<uint64_t>(SZ(s) + 1);
		vector<uint64_t> hash2
			= vector<uint64_t>(SZ(s) + 1);
		REP(i, SZ(s)) {
			hash1[i + 1] = add(mul(hash1[i], base1), s[i]);
			hash2[i + 1] = add(mul(hash2[i], base2), s[i]);
		}
		return { hash1, hash2 };
	}

	/// @brief 与えられたvectorについてhash値を求める
	/// @tparam T vectorの要素の型
	/// @param s hash値を求めるvector
	/// @return 計算された2通りのhash値
	template<typename T>
	pair<vector<uint64_t>, vector<uint64_t>> build(
		const vector<T>& s) {
		expand(SZ(s) + 1);
		vector<uint64_t> hash1
			= vector<uint64_t>(SZ(s) + 1);
		vector<uint64_t> hash2
			= vector<uint64_t>(SZ(s) + 1);
		REP(i, SZ(s)) {
			hash1[i + 1] = add(mul(hash1[i], base1), s[i]);
			hash2[i + 1] = add(mul(hash2[i], base2), s[i]);
		}
		return { hash1, hash2 };
	}

	/// @brief build関数で計算されたhash値をもとにbegin文字目からlength文字のhash値を求める
	/// @param hash build関数の結果
	/// @param begin 文字列の初期位置(0-indexed)
	/// @param length 文字列の長さ
	/// @return 2通りのhash値
	pair<uint64_t, uint64_t> query(
		const pair<vector<uint64_t>, vector<uint64_t>>&
			hash,
		int begin, int length) {
		assert(begin + length <= SZ(hash.first));
		assert(begin >= 0);
		assert(length > 0);
		expand(length);
		return {
			add(hash.first[begin + length],
				mod - mul(hash.first[begin], pow1[length])),
			add(hash.second[begin + length],
				mod - mul(hash.second[begin], pow2[length]))
		};
	}
};