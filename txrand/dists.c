#include "txrand.h"
#include <errno.h>
#include <math.h>

/* cnt ~ B(n,p)*/
unsigned binomdist(const unsigned n,const double p) {
	//�����������һ�����Ӷ���ֲ������ֵ
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
	return cnt;
}

#define ZIGGURAT_TABLE_SIZE 256

double normdist(const double average, const double stddev) {
    // --- ��̬�洢 Ziggurat ��Ͳ��� ---
    
    static double x[ZIGGURAT_TABLE_SIZE + 1]; // �ֲ��ұ߽�
    static double y[ZIGGURAT_TABLE_SIZE];     // �ֲ�PDFֵ
    static double r;                          // β���ֲ��ұ߽�
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;

        // --- ��̬���� r �� v ---
        double v;
        r = 3.0; // ��ʼ�²�
        do {
            // ����β���ֲ���� v = r * f(r) + ��_r^�� f(x) dx
            double f_r = exp(-0.5 * r * r);
            v = r * f_r + sqrt(2 * acos(-1.0)) * (1.0 - 0.5 * erfc(-r / sqrt(2)));
            r += 0.0001; // �𲽱ƽ�
        } while (v > 1.0 / ZIGGURAT_TABLE_SIZE); // ֱ�� v �� 1/256

        // --- ��ʼ�� Ziggurat �� ---
        x[0] = v / exp(-0.5 * r * r);
        x[1] = r;
        y[0] = exp(-0.5 * r * r);

        for (int i = 2; i < ZIGGURAT_TABLE_SIZE; i++) {
            x[i] = sqrt(-2 * log(v / x[i - 1] + exp(-0.5 * x[i - 1] * x[i - 1])));
            y[i - 1] = exp(-0.5 * x[i - 1] * x[i - 1]);
        }
        x[ZIGGURAT_TABLE_SIZE] = 0.0;
        y[ZIGGURAT_TABLE_SIZE - 1] = 1.0;
    }

    // --- Ziggurat ���� ---
    while (1) {
        // 1. ѡ�����
        double u1 = random();
        int j = (int)(u1 * ZIGGURAT_TABLE_SIZE);
        if (j >= ZIGGURAT_TABLE_SIZE) j = ZIGGURAT_TABLE_SIZE - 1;

        // 2. ���ɲ��ں�����
        double u2 = random();
        double x_j = x[j];
        double x_val = u2 * x_j;

        // 3. ���ٽ��ܣ�������ȫ�������£�
        if (x_val < x[j + 1]) {
            return average + stddev * ((u1 < 0.5) ? x_val : -x_val);
        }

        // 4. ����β����ܾ�����
        if (j == 0) {
            // β���ֲ���Marsaglia ������
            double u3, v;
            do {
                u3 = random();
                v = -log(random()) / r;
            } while (u3 * u3 > exp(-0.5 * v * v));
            return average + stddev * ((u2 < 0.5) ? r + v : -r - v);
        }
        else {
            // �ܾ�����
            double y_j = y[j];
            double y_val = y_j + random() * (y[j - 1] - y_j);
            if (y_val < exp(-0.5 * x_val * x_val)) {
                return average + stddev * ((u1 < 0.5) ? x_val : -x_val);
            }
        }
    }
}

float normdistf(const float average, const float stddev) {
    // --- ��̬�洢 Ziggurat ��Ͳ��� ---
#define ZIGGURAT_TABLE_SIZE 256
    static float x[ZIGGURAT_TABLE_SIZE + 1]; // �ֲ��ұ߽�
    static float y[ZIGGURAT_TABLE_SIZE];     // �ֲ�PDFֵ
    static float r;                          // β���ֲ��ұ߽�
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        // --- ��̬���� r �� v ---
        float v;
        r = 3.0f; // ��ʼ�²�
        do {
            float f_r = expf(-0.5f * r * r);
            v = r * f_r + sqrtf(2.0f * acosf(-1.0f)) * (1.0f - 0.5f * erfcf(-r / sqrtf(2.0f)));
            r += 0.0001f; // �𲽱ƽ�
        } while (v > 1.0f / ZIGGURAT_TABLE_SIZE);

        // --- ��ʼ�� Ziggurat �� ---
        x[0] = v / expf(-0.5f * r * r);
        x[1] = r;
        y[0] = expf(-0.5f * r * r);
        for (int i = 2; i < ZIGGURAT_TABLE_SIZE; i++) {
            x[i] = sqrtf(-2.0f * logf(v / x[i - 1] + expf(-0.5f * x[i - 1] * x[i - 1])));
            y[i - 1] = expf(-0.5f * x[i - 1] * x[i - 1]);
        }
        x[ZIGGURAT_TABLE_SIZE] = 0.0f;
        y[ZIGGURAT_TABLE_SIZE - 1] = 1.0f;
    }

    // --- Ziggurat ���� ---
    while (1) {
        float u1 = randomf();
        int j = (int)(u1 * ZIGGURAT_TABLE_SIZE);
        if (j >= ZIGGURAT_TABLE_SIZE) j = ZIGGURAT_TABLE_SIZE - 1;

        float u2 = randomf();
        float x_j = x[j];
        float x_val = u2 * x_j;

        if (x_val < x[j + 1])
            return average + stddev * ((u1 < 0.5f) ? x_val : -x_val);

        if (j == 0) {
            float u3, v;
            do {
                u3 = randomf();
                v = -logf(randomf()) / r;
            } while (u3 * u3 > expf(-0.5f * v * v));
            return average + stddev * ((u2 < 0.5f) ? r + v : -r - v);
        }
        else {
            float y_j = y[j];
            float y_val = y_j + randomf() * (y[j - 1] - y_j);
            if (y_val < expf(-0.5f * x_val * x_val))
                return average + stddev * ((u1 < 0.5f) ? x_val : -x_val);
        }
    }
}

long double normdistl(const long double average, const long double stddev) {
    // --- ��̬�洢 Ziggurat ��Ͳ��� ---
#define ZIGGURAT_TABLE_SIZE 256
    static long double x[ZIGGURAT_TABLE_SIZE + 1]; // �ֲ��ұ߽�
    static long double y[ZIGGURAT_TABLE_SIZE];     // �ֲ�PDFֵ
    static long double r;                          // β���ֲ��ұ߽�
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        // --- ��̬���� r �� v ---
        long double v;
        r = 3.0L; // ��ʼ�²�
        do {
            long double f_r = expl(-0.5L * r * r);
            v = r * f_r + sqrtl(2.0L * acosl(-1.0L)) * (1.0L - 0.5L * erfcl(-r / sqrtl(2.0L)));
            r += 0.0001L; // �𲽱ƽ�
        } while (v > 1.0L / ZIGGURAT_TABLE_SIZE);

        // --- ��ʼ�� Ziggurat �� ---
        x[0] = v / expl(-0.5L * r * r);
        x[1] = r;
        y[0] = expl(-0.5L * r * r);
        for (int i = 2; i < ZIGGURAT_TABLE_SIZE; i++) {
            x[i] = sqrtl(-2.0L * logl(v / x[i - 1] + expl(-0.5L * x[i - 1] * x[i - 1])));
            y[i - 1] = expl(-0.5L * x[i - 1] * x[i - 1]);
        }
        x[ZIGGURAT_TABLE_SIZE] = 0.0L;
        y[ZIGGURAT_TABLE_SIZE - 1] = 1.0L;
    }

    // --- Ziggurat ���� ---
    while (1) {
        long double u1 = randoml();
        int j = (int)(u1 * ZIGGURAT_TABLE_SIZE);
        if (j >= ZIGGURAT_TABLE_SIZE) j = ZIGGURAT_TABLE_SIZE - 1;

        long double u2 = randoml();
        long double x_j = x[j];
        long double x_val = u2 * x_j;

        if (x_val < x[j + 1])
            return average + stddev * ((u1 < 0.5L) ? x_val : -x_val);

        if (j == 0) {
            long double u3, v;
            do {
                u3 = randoml();
                v = -logl(randoml()) / r;
            } while (u3 * u3 > expl(-0.5L * v * v));
            return average + stddev * ((u2 < 0.5L) ? r + v : -r - v);
        }
        else {
            long double y_j = y[j];
            long double y_val = y_j + randoml() * (y[j - 1] - y_j);
            if (y_val < expl(-0.5L * x_val * x_val))
                return average + stddev * ((u1 < 0.5L) ? x_val : -x_val);
        }
    }
}