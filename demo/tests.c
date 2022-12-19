#include <stdio.h>
#include <stdlib.h>
#include "txrand.h"
#include "private/tests.h"
#include <assert.h>
void randbool_b_test(void)
{
	char s[] = "true/all:%f\n";
	for (int i = 0; i < 25; i++)
	{
		if (randbool_b()) printf("true\t");
		else printf("0\t");
		if (!((i + 1) % 5)) putchar('\n');
	}
	int trueCount = 0;
	for (int i = 0; i < 1000000; i++)
	{
		if (randbool_b())  trueCount++;
	}
	printf(s, (double)trueCount / 1000000);
}
void fillbuffer_test(void)
{
	unsigned char buffer[16];
	fillbuffer(buffer, sizeof(char), sizeof(buffer) / sizeof(char));
	for (int i = 0; i < 16; i++) 
		printf("%02x  ", buffer[i]);
	putchar('\n');
}
void choice_test(void)
{
	int arr[20], i, r;
	for (i = 0; i < 20; i++) {
		arr[i] = randint(1, 100);
		printf("%d ", arr[i]);
	}
	putchar('\n');
	r = *(int*)choice(arr, sizeof r, sizeof arr / sizeof r);
	printf("result:%d\n", r);

}
void random_test(void) {
	for (int i = 0; i < 10; i++) 
		printf("%lf\n",random());
	
}
void randbool_test(void) {
	const double w = random();
	int x[] = { 0, 0 };
	_Bool r = 0;
	for (int i = 0; i < 1000000; i++) {
		r = randbool(w);

		if (r) x[1]++;
		else x[0]++;
	}
	;
	const double aw = (double)x[1] / (double)1000000;
	printf("weight:%lf\nactual weight%lf\n",w,aw);
}
