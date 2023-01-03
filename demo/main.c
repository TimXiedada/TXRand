#include "txrand.h"
#include <stdio.h>
#include <Windows.h>
#include "private/tests.h"
int main(void) 
{
    randbool_b_test();
    fillbuffer_test();
    choice_test();
    random_test();
    randbool_test();
    sample_test();
    shuffle_test();

    randint_randbelow_test();
    return 0;
}