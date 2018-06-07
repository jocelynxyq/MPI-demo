#include <stdio.h>
#include <stdlib.h>

int compute(int len, int *a, int x)
{
	int sum = a[0];
	int partX = x;
	for (int i = 1; i < len; i++)
	{
		sum += a[i] * partX;
		partX *= x;
	}
	return sum;
}

int main()
{
	int A[3] = {1,2,3};
	int len = sizeof(A) / sizeof(int);
	int result = compute(len, A, 2);
	printf("result = %d", result);
	system("pause");
	return 0;
}