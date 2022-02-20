#include "math-helpers.h"

int pow2(int n)
{
	int res = 1;
	for (int i = 0; i < n; i++)
	{
		res <<= 1;
	}
	return res;
}