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

// ������A������[p, r]���ҳ���iС����
// ƽ��ʱ�临�Ӷ��� O(n)���ʱ�临�Ӷ���O(n^2)
// 
int randomized_select(int A[], int p, int r, int i)
{
	// �ҵ������pivot
	int q = randomized_partition(A, p, r);
	// ������ĳ��ȣ�����pivot
	int k = q-p+1;
	if (i == k) {			// pivot���Ǵ�
		return A[q];
	} else if ( i < k) {    // ����������ҵ�iС��Ԫ�� 
		return randomized_select(A, p, q-1, i);
	} else {                // ����������ҵ�(i-k)С��Ԫ��
		return randomized_select(A, q+1, r, i-k);
	}
}

int randomized_select(int A[], int n, int i)
{
	return randomized_select(A, 1, n, i);
}