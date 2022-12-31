#include "daylight/base.hpp"

struct my_rnd {
	mt19937_64 randomEngine;
	my_rnd() {
		random_device randomDevice;
		vector<uint32_t> randomSeedVector(10);
		generate(randomSeedVector.begin(),
				 randomSeedVector.end(), ref(randomDevice));
		seed_seq randomSeed(randomSeedVector.begin(),
							randomSeedVector.end());
		randomEngine = mt19937_64(randomSeed);
	}

	/// @brief 0以上upper未満の整数を一つランダムに取得する
	/// @param upper 乱数の上限値(exclusive)
	/// @return 0以上upper未満のランダムな整数
	int nextInt(int upper) {
		uniform_int_distribution<int> randomIntDistribution(
			0, upper - 1);
		return randomIntDistribution(randomEngine);
	}

	/// @brief lower以上upper未満の整数を一つランダムに取得する
	/// @param lower 乱数の下限値(inclusive)
	/// @param upper 乱数の上限値(exclusive)
	/// @return lower以上upper未満のランダムな整数
	int nextInt(int lower, int upper) {
		uniform_int_distribution<int> randomIntDistribution(
			lower, upper - 1);
		return randomIntDistribution(randomEngine);
	}

	/// @brief 0以上upper未満の整数を一つランダムに取得する
	/// @param upper 乱数の上限値(exclusive)
	/// @return 0以上upper未満のランダムな整数
	ll nextLong(ll upper) {
		uniform_int_distribution<ll> randomIntDistribution(
			0, upper - 1);
		return randomIntDistribution(randomEngine);
	}

	/// @brief lower以上upper未満の整数を一つランダムに取得する
	/// @param lower 乱数の下限値(inclusive)
	/// @param upper 乱数の上限値(exclusive)
	/// @return lower以上upper未満のランダムな整数
	ll nextLong(ll lower, ll upper) {
		uniform_int_distribution<ll> randomIntDistribution(
			lower, upper - 1);
		return randomIntDistribution(randomEngine);
	}

	/// @brief 0以上1未満の実数をランダムに取得する
	/// @return 0以上1未満のランダムな実数
	double next() {
		uniform_real_distribution<double>
			randomDoubleDistribution(0.0, 1.0);
		return randomDoubleDistribution(randomEngine);
	}

	/// @brief 0以上upper未満の実数をランダムに取得する
	/// @param upper 乱数の上限値(exclusive)
	/// @return 0以上upper未満のランダムな実数
	double nextDouble(double upper) {
		uniform_real_distribution<double>
			randomDoubleDistribution(0.0, upper);
		return randomDoubleDistribution(randomEngine);
	}
};