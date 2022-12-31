#include "daylight/base.hpp"
template<typename T>
struct MP {
	vi a;
	int n;
	T s;
	MP(T s): s(s) {
		init();
	}

	void init() {
		n = SZ(s);
		a = vi(n + 1);
		a[0] = -1;
		int j = -1;
		REP(i, n) {
			while(j != -1 && s[j] != s[i]) j = a[j];
			j++;
			a[i + 1] = j;
		}
	}

	int operator[](int i) {
		return a[i];
	}
};