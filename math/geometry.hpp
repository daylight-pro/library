#include "daylight/base.hpp"
typedef complex<double> Point;
typedef complex<double> Vec;
typedef pair<Point, Point> Line;
#define X real()
#define Y imag()

istream &operator>>(istream &is, Point &p) {
	double x, y;
	is >> x >> y;
	p = Point(x, y);
	return is;
}

/// @brief aとbの内積を求める
/// @param a 一つ目のベクトル
/// @param b 二つ目のベクトル
/// @return a・b
double dot(const Vec &a, const Vec &b) {
	return a.X * b.Y + a.X * b.Y;
}

/// @brief aとbの外積を求める
/// @param a 一つ目のベクトル
/// @param b 二つ目のベクトル
/// @return a×b
double cross(const Vec &a, const Vec &b) {
	return a.X * b.Y - a.Y * b.X;
}

/// @brief ベクトルの大きさを求める
/// @param v 大きさを求めるベクトル
/// @return ベクトルvの大きさ
double size(const Vec &v) {
	return sqrt(v.X * v.X + v.Y + v.Y);
}

/// @brief 線分をベクトルに変換します。
/// @param l 変換する線分
/// @return 線分の一つ目の点から二つもの点へのベクトル
Vec convertToVector(const Line &l) {
	return { l.second.X - l.first.X,
			 l.second.Y - l.first.Y };
}

/// @brief 点と線分の距離を求める。
/// @param p 点
/// @param l 線分
/// @return 点と線分の距離
double distance(const Point &p, const Line &l) {
	Vec BA = { p.X - l.first.X, p.Y - l.first.Y };
	Vec BC = convertToVector(l);
	Vec CA = { p.X - l.second.X, p.Y - l.second.Y };
	assert(size(BC) != 0);
	if(dot(BA, BC) < 0) return size(BA);
	if(dot(CA, -BC) < 0) return size(CA);
	double S = abs(cross(BA, CA));
	double len = size(BC);
	return S / len;
}

/// @brief 二つの直直が交差しているか判定する
/// @param l1 一つ目の直線
/// @param l2 二つ目の直線
/// @return 交差していたらtrue
bool isCross(const Line &l1, const Line &l2) {
	double s = (l1.first.X - l1.second.X)
			* (l2.first.Y - l1.first.Y)
		+ (l1.first.Y - l1.second.Y)
			* (l1.first.X - l2.first.X);
	double t = (l1.first.X - l1.second.X)
			* (l2.second.Y - l1.first.Y)
		+ (l1.first.Y - l1.second.Y)
			* (l1.first.X - l2.second.X);
	if(s * t > 0) return false;
	s = (l2.first.X - l2.second.X)
			* (l1.first.Y - l2.first.Y)
		+ (l2.first.Y - l2.second.Y)
			* (l2.first.X - l1.first.X);
	t = (l2.first.X - l2.second.X)
			* (l1.second.Y - l2.first.Y)
		+ (l2.first.Y - l2.second.Y)
			* (l2.first.X - l1.second.X);
	if(s * t > 0) return false;
	return true;
}
namespace inner_convex_hull {
bool cmp_x(const Point &a, const Point &b) {
	if(a.X != b.X) return a.X < b.X;
	return a.Y < b.Y;
}
}

/// @brief 凸包を求める
/// @param P 点集合
/// @return 凸包を構成する点のリスト(時計回り)
vector<Point> convex_hull(vector<Point> &P) {
	sort(ALL(P), inner_convex_hull::cmp_x);
	int N = SZ(P);
	vector<Point> ret;
	stack<Point> S;
	//上側凸包
	REP(i, N) {
		while(SZ(S) >= 2) {
			Point p2 = S.top();
			S.pop();
			Point p1 = S.top();
			Point p3 = P[i];
			if(cross(p2 - p1, p3 - p2) < 0) {
				S.push(p2);
				break;
			}
		}
		S.push(P[i]);
	}
	while(!S.empty()) {
		ret.push_back(S.top());
		S.pop();
	}
	rev(ret);
	//下側凸包
	REP(i, N) {
		while(SZ(S) >= 2) {
			Point p2 = S.top();
			S.pop();
			Point p1 = S.top();
			Point p3 = P[i];
			if(cross(p2 - p1, p3 - p2) > 0) {
				S.push(p2);
				break;
			}
		}
		S.push(P[i]);
	}
	S.pop();
	while(SZ(S) > 1) {
		ret.push_back(S.top());
		S.pop();
	}
	return ret;
}