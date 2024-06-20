#include "daylight/base.hpp"
struct RollingHash {
private:
	static const uint64_t mod = (1ull << 61ull) - 1;
	using uint128_t = __uint128_t;
	static inline uint64_t base1 = 0, base2 = 0;
	static inline vector<uint64_t> pow1, pow2;
	vector<uint64_t> hash1, hash2;

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
	/// @brief 指定した文字列に対して,ハッシュを前計算する
	/// @param s ハッシュを取る文字列
	RollingHash(const string& s)
		: RollingHash(vector<char>(s.begin(), s.end())) {
	}

	/// @brief 指定した配列に対して,ハッシュを前計算する
	/// @param s ハッシュを取る配列
	template<typename T>
	RollingHash(const vector<T>& s) {
		if(base1 == 0) {
			mt19937_64 mt(chrono::steady_clock::now()
							  .time_since_epoch()
							  .count());
			uniform_int_distribution<uint64_t> rand(
				1e9, RollingHash::mod - 1);
			base1 = rand(mt);
			base2 = rand(mt);
		}
		pow1.push_back(1);
		pow2.push_back(1);
		expand(SZ(s) + 1);
		hash1 = vector<uint64_t>(SZ(s) + 1);
		hash2 = vector<uint64_t>(SZ(s) + 1);
		REP(i, SZ(s)) {
			hash1[i + 1] = add(mul(hash1[i], base1), s[i]);
			hash2[i + 1] = add(mul(hash2[i], base2), s[i]);
		}
	}

	/// @brief build関数で計算されたhash値をもとにbegin文字目からlength文字のhash値を求める
	/// @param begin 文字列の初期位置(0-indexed)
	/// @param length 文字列の長さ
	/// @return 2通りのhash値
	pair<uint64_t, uint64_t> query(int begin, int length) {
		assert(begin + length <= SZ(hash1));
		assert(begin >= 0);
		assert(length > 0);
		expand(length);
		return {
			add(hash1[begin + length],
				mod - mul(hash1[begin], pow1[length])),
			add(hash2[begin + length],
				mod - mul(hash2[begin], pow2[length]))
		};
	}
};