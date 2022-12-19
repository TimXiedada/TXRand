#include "txrand.h"
#include <stdio.h>
#include <Windows.h>
#include "private/tests.h"
int main(void) 
{
	/*
	bool succ = txrand_init();
	if (!succ) {
		printf("TXRand³õÊ¼»¯Ê§°Ü£¡´íÎóÎª%#08x",GetLastError()); return 1;
	}
	*/
	randbool_b_test();
	fillbuffer_test();
	choice_test();
	random_test();
	randbool_test();
	//randint_randbelow_test();
	sample_test();
	shuffle_test();
	//txrand_cleanup();
	return 0;
}