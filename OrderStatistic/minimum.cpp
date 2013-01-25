#include <assert.h>

// 返回数组A中最小的数
// n-1次比较，已经是最优时间复杂度
int minimum(int A[], int n)
{
	assert(A && (n>0));
	int min = A[1];
	for (int i=2;i<=n;i++) {
		if (A[i] < min)
			min = A[i];
	}
	return min;
}