#include "daylight/base.hpp"

class dsu {
private:
	vi p;  //p => 親
	vi time;
	vector<vector<PI>> size;
	int N;
	int now;

public:
	dsu() {
	}
	/// @brief サイズnで辺がないUFを生成
	/// @param sz 頂点数
	dsu(int sz) {
		N = sz;
		now = 0;
		p.resize(N, 1);
		time.resize(N, INF);
		size.resize(N, vector<PI>(1, { 0, 1 }));
	}

	/// @brief 時刻tの時点でxとyが同じ連結成分に属しているか判定
	/// @param x 頂点1
	/// @param y 頂点2
	/// @param t 判定する時間
	/// @return 同一連結成分ならtrue
	bool same(int x, int y, int t) {
		return leader(x, t) == leader(y, t);
	}

	/// @brief 時刻を1進めてxとyをマージする
	/// @param x 頂点1
	/// @param y 頂点2
	/// @return マージ後の時刻
	int merge(int x, int y) {
		++now;
		x = leader(x, now);
		y = leader(y, now);
		if(x == y) return now;
		if(p[x] < p[y]) {
			swap(x, y);
		}
		p[x] += p[y];
		size[x].push_back({ now, p[x] });
		p[y] = x;
		time[y] = now;
		return now;
	}

	/// @brief 時刻t時点でのxが属する連結成分のリーダーを取得
	/// @param x 頂点
	/// @param t 時刻
	/// @return リーダーの頂点番号
	int leader(int x, int t) {
		if(time[x] > t)
			return x;
		else
			return leader(p[x], t);
	}

	/// @brief 現在の時刻を取得する
	/// @return 現在の時刻
	int get_time() {
		return now;
	}

	/// @brief 時刻t時点でのxが属する連結成分のサイズを求める
	/// @param x 頂点番号
	/// @param t 時刻
	/// @return 連結成分のサイズ
	int get_size(int x, int t) {
		x = leader(x, t);
		auto it = upper_bound(ALL(size[x]), PI { t, 0 });
		it--;
		return (*it).second;
	};
};