#include <stdio.h>
#include "myComplex.h"

void complex_print(struct Complex compl)
{
	printf("Real: %d\n", compl.real);
	printf("Img: %d\n", compl.img);
}
