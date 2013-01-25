#include "stdafx.h"

void swap(int A[], int i, int j)
{
	assert(A && (i<=j));
	if (i==j)
		return;

	A[i] ^= A[j];
	A[j] = A[i] ^ A[j];
	A[i] = A[i] ^ A[j];
}

int partition(int A[], int p, int r)
{
	assert(A &&(p<=r));

	int i = p-1;
	for (int j=p;j<r;j++) {
		if (A[j] <= A[r]) { 
			i++;
			swap(A, i, j);
		}
	}

	swap(A, i+1, r);
	return i+1;
}

int randomized_partition(int A[], int p, int r)
{
	int i = rand()%(r-p+1)+p;

	swap(A, i, r);
	return partition(A, p, r);
}

// 从数组A的区间[p, r]中找出第i小的数
// 平均时间复杂度是 O(n)，最坏时间复杂度是O(n^2)
// 
int randomized_select(int A[], int p, int r, int i)
{
	// 找到随机的pivot
	int q = randomized_partition(A, p, r);
	// 左区间的长度，包括pivot
	int k = q-p+1;
	if (i == k) {			// pivot就是答案
		return A[q];
	} else if ( i < k) {    // 在左区间查找第i小的元素 
		return randomized_select(A, p, q-1, i);
	} else {                // 在右区间查找第(i-k)小的元素
		return randomized_select(A, q+1, r, i-k);
	}
}

int randomized_select(int A[], int n, int i)
{
	return randomized_select(A, 1, n, i);
}