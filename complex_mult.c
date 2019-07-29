#include "myComplex.h"

struct Complex complex_mult(struct Complex compl1, struct Complex compl2)
{
	struct Complex res;

	res.real = compl1.real * compl2.real - compl1.img * compl2.img;
	res.img = compl1.real * compl2.img + compl1.img * compl2.real;

	return res;
}
