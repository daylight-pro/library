template<typename T>
struct RangeSum {
private:
	vector<T> V;
	int N = -1;

public:
	RangeSum(vector<T> &v) {
		N = SZ(v);
		V = vector<T>(N + 1);
		V[0] = T(0);
		REP(i, N) {
			V[i + 1] = v[i] + V[i];
		}
	}

	/// @brief 区間[l,r)の和を求める
	/// @param l 区間の左端(inclusive)
	/// @param r 区間の右端(exclusive)
	/// @return 区間[l,r)の和
	T sum(int l, int r) {
		chmax(l, 0);
		chmin(r, N);
		chmax(r, l);
		return V[r] - V[l];
	}
};

template<typename T>
struct RangeSum2D {
private:
	vector<vector<T>> V;
	int H = -1;
	int W = -1;

public:
	RangeSum2D(vector<vector<T>> &v) {
		H = SZ(v);
		W = SZ(v[0]);
		V = vector<vector<T>>(H, vector<T>(W));
		REP(i, H) {
			REP(j, W) {
				V[i][j] += v[i][j];
				if(i != 0) V[i][j] += V[i - 1][j];
				if(j != 0) V[i][j] += V[i][j - 1];
				if(i != 0 && j != 0)
					V[i][j] -= V[i - 1][j - 1];
			}
		}
	}

	/// @brief 2点を対角に持つ長方形領域の和を計算する(2点とも含む)
	/// @param y1 左上のy座標
	/// @param x1 左上のx座標
	/// @param y2 右下のy座標
	/// @param x2 右下のx座標
	/// @return 長方形領域の和
	T sum(int y1, int x1, int y2, int x2) {
		if(y1 > y2 || x1 > x2)return T(0);
		T ret = V[y2][x2];
		if(y1 != 0) ret -= V[y1 - 1][x2];
		if(x1 != 0) ret -= V[y2][x1 - 1];
		if(y1 != 0 && x1 != 0) ret += V[y1 - 1][x1 - 1];
		return ret;
	}
};