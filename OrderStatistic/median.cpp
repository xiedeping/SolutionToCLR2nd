#include "stdafx.h"
#include "randomized_select.h"

int median(int A[], int n)
{
	return randomized_select(A, n, (1+n)/2);
}

int find_median(int A[], int B[], int p, int r, int n)
{
	assert(A && B && p>0 && r>0 && n>0);

	if (p > r) 
		return -1;

	int k = (p+r)/2;
	if (k == n) {
		if (A[n] <= B[1])
			return k;
		else 
			return find_median(A, B, p, k-1, n);
	}
	
	if (A[k] >= B[n-k] && A[k] <= B[n-k+1]) {
		return k;
	}
	if (A[k] < B[n-k]) {
		return find_median(A, B, k+1, r, n);
	}
	if (A[k] > B[n-k+1]) {
		return find_median(A, B, p, k-1, n);
	}
}

// 从有序数组A和B中找出中位数
// 平均时间复杂度O(lgN)，最坏时间复杂度O(lgN)
int median_two_array(int A[], int B[], int n)
{
	assert(A && B && n>0);
	int m = find_median(A, B, 1, n, n);
	if (-1 != m)
		return m;
	return find_median(B, A, 1, n, n);
}