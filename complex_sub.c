#include "myComplex.h"

struct Complex complex_sub(struct Complex compl1, struct  Complex compl2)
{
	struct Complex res;
	
	res.real = compl1.real - compl2.real;
	res.img = compl1.img - compl2.img;

	return res;
}

