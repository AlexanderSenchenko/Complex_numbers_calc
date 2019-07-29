#include "myComplex.h"

struct Complex complex_add(struct Complex compl1, struct Complex compl2)
{
	struct Complex sum;
	
	sum.real = compl1.real + compl2.real;
	sum.img = compl1.img + compl2.img;
	return sum;
}
