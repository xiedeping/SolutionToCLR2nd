#include "stdafx.h"
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

// ��������A��������
// n-1�αȽϣ��Ѿ�������ʱ�临�Ӷ�
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

// ��������A�е���С���������
// ���nΪ�������Ƚϴ���Ϊ (n/2)���½� * 3��
// ���nΪż�����Ƚϴ���Ϊ (n/2)*3 - 2��
// �Ѿ�������ʱ�临�Ӷ�
void minmax(int A[], int n, int &min, int &max)
{
	assert(A && (n>0));

	int i;
	// ����n����ż����������γ�ʼ��min��max
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

	// ÿ��ȡ�������֣�����С����min�Ƚϣ����ϴ����max�Ƚϣ�Ȼ�����min��max
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