#include "txrand.h"
_Bool TXRANDAPI  randbool(double weight) {
	const double i = random();
	_Bool returnvalue = (i < weight);
	return returnvalue;
}