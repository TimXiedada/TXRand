#include "private/tests.h"
#include <assert.h>
void randint_randbelow_test(void) {
    const unsigned TIMES = 500000000;
    unsigned i; 
    {
        void*  s = malloc(1);
        srand(s);
        free(s); 
    }
    int max4rbt = rand();
    int t;
    for (i = 0; i < TIMES; i++) {
        t = randbelow(max4rbt);
        assert(t < max4rbt&& t >= 0);
    }
    int max4rit = rand(), min4rit = rand();
retry:
    if (randbool_b()) min4rit = -min4rit;
    if (max4rit < min4rit)goto retry;
    for (i = 0; i < TIMES; i++) {
        t = randint(max4rit, min4rit);
        assert(t < max4rit&& t >= min4rit);
    }
    
}


void rand_float_test(void) {
test_float: {
        
    }
}