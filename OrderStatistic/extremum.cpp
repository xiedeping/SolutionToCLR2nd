#include "stdafx.h"
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

// 返回数组A中最大的数
// n-1次比较，已经是最优时间复杂度
int maximum(int A[], int n)
{
	assert(A && (n>0));

	int max = A[1];
	for (int i=2;i<=n;i++) {
		if (A[i] > max)
			max = A[i];
	}
	return max;
}

// 返回数组A中的最小数和最大数
// 如果n为奇数，比较次数为 (n/2)的下界 * 3；
// 如果n为偶数，比较次数为 (n/2)*3 - 2。
// 已经是最优时间复杂度
void minmax(int A[], int n, int &min, int &max)
{
	assert(A && (n>0));

	int i;
	// 根据n的奇偶性来决定如何初始化min和max
	if (n%2) {
		min = A[1];
		max = min;
		i = 2;
	} else {
		if (A[1] < A[2]) {
			min = A[1];
			max = A[2];
		} else {
			min = A[2];
			max = A[1];
		}
		i = 3;
	}

	// 每次取两个数字，将较小的与min比较，将较大的与max比较，然后更新min和max
	for (; i<=n; i+=2) {
		if (A[i] < A[i+1]) {
			if (A[i] < min)
				min = A[i];
			if (A[i+1] > max)
				max = A[i+1];
		} else {
			if (A[i+1] < min)
				min = A[i+1];
			if (A[i] > max)
				max = A[i];
		}
	}
}