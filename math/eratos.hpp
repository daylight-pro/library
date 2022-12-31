#include "daylight/base.hpp"
void eratos(int n, vb& isprime) {
	isprime = vb(n + 1, true);
	isprime[0] = false;
	isprime[1] = false;
	int last = ((int) sqrt(n)) + 1;
	FOR(i, 2, last) {
		if(isprime[i]) {
			int j = i + i;
			while(j <= n) {
				isprime[j] = false;
				j += i;
			}
		}
	}
}