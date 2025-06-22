#include "txrand.h"
#include <errno.h>


/* cnt ~ B(n,p)*/
unsigned binomdist(const unsigned n,const double p) {
	//这个函数生成一个服从二项分布的随机值
	unsigned cnt = 0, i;
	if (!n || p == 0.0) return 0;
	if (p == 1.0) return n;
	if (p < 0.0 || p > 1.0) {
		errno = ERANGE;
		return 0;
	}
	for (i = 0; i < n; i++) {
		if (randbool_w(p)) cnt += 1;
	}
	return cnt;
}

unsigned long binomdistl(const unsigned long n, const double p) {
	unsigned long cnt = 0, i;
	if (!n || p == 0.0) return 0;
	if (p == 1.0) return n;
	if (p < 0.0 || p > 1.0) {
		errno = ERANGE;
		return 0;
	}
	for (i = 0; i < n; i++) {
		if (randbool_w(p)) cnt += 1;
	}
	return cnt;
}

unsigned long long binomdistll(const unsigned long long n, const double p) {
	unsigned long long cnt = 0, i;
	if (!n || p == 0.0) return 0;
	if (p == 1.0) return n;
	if (p < 0.0 || p > 1.0) {
		errno = ERANGE;
		return 0;
	}
	for (i = 0; i < n; i++) {
		if (randbool_w(p)) cnt += 1;
	}
	return cnt;
}

unsigned short binomdists(const unsigned short n, const double p) {
	unsigned short cnt = 0, i;
	if (!n || p == 0.0) return 0;
	if (p == 1.0) return n;
	if (p < 0.0 || p > 1.0) {
		errno = ERANGE;
		return 0;
	}
	for (i = 0; i < n; i++) {
		if (randbool_w(p)) cnt += 1;
	}
	return cnt;
}


unsigned hypogeomdist(const unsigned N, const unsigned M, const unsigned n) {
	unsigned cur_N = N, cur_M = M, i,index,cnt = 0;
	for (i = 0; i < n; i++) {
		index = randbelowu(cur_N); 
		if (cur_M > 0) {
			if (index < cur_M) { 
				cnt ++;
				cur_M--;
			}
		}else{
			cur_N--;
		}
	}
}