#include <assert.h>

// ��������A����С����
// n-1�αȽϣ��Ѿ�������ʱ�临�Ӷ�
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