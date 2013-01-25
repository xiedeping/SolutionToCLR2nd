// LIS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define NUM 8

// 方案一
void lis_1(int A[]);

// 方案二
void lis_2(int A[]);

int _tmain(int argc, _TCHAR* argv[])
{
	int A[] = {0, 1, -1, 2, -3, 4, -5, 6, -7};
	lis_1(A);
	lis_2(A);

	return 0;
}
void print_lis_1(int A[], int PRE[], int i);

void lis_1(int A[])
{
	// 保存子问题的最优解
	int LIS[NUM+1];

	// 保存最优解的构造过程
	// 其实是最长递增子序列的前一个元素下标
	int PRE[NUM+1];
	
	for (int i=1;i<=NUM;i++) {
		LIS[i] = 1;
		PRE[i] = 0;
		// 这里存在优化空间，虽然不能降级，但可以减低常数因子
		for (int k=1;k<i;k++) {
			if ((A[i] > A[k]) && (LIS[k] + 1 > LIS[i])) {
				LIS[i] = LIS[k] + 1;
				PRE[i] = k;
			}
		}

		// 没有找到k，A[i]是新的递增子序列的开始
		if (0 == PRE[i])
			PRE[i] = i;
	}

	printf("LIS[i]:\n");
	for (int i=1;i<=NUM;i++)
		printf("%d ", LIS[i]);
	printf("\n");

	printf("PRE[i]:\n");
	for (int i=1;i<=NUM;i++)
		printf("%d ", PRE[i]);
	printf("\n");

	int max = 1;
	for (int i=1;i<=NUM;i++) {
		if (A[i] > A[max])
			max = i;
	}

	printf("LIS: ");
	print_lis_1(A, PRE, max);
	printf("\n");
}

// 打印最靠后的最长递增子序列
void print_lis_1(int A[], int PRE[], int i)
{
	if (PRE[i] == i) {
		printf("%d  ", A[i]);
	} else {
		print_lis_1(A, PRE, PRE[i]);
		printf("%d  ", A[i]);
	}
}

void lis_2(int A[])
{
	int LIS[NUM+1];
	int maxV[NUM+1];
	

	// 保存长度为i的递增子序列最后元素的最小值
	maxV[1] = A[1];
	for (int i=2;i<=NUM;i++)
		maxV[i] = INT32_MIN;

	int maxLIS = 1;
	for (int i=1;i<=NUM;i++) {
		LIS[i] = 1;
		// maxV其实是个递增数组，可以二分查找
		for (int k=maxLIS; k>0;k--) {
			if (A[i] > maxV[k]) {
				LIS[i] = k+1;
				
				if (k+1 > maxLIS) {
					maxLIS = k + 1;
					maxV[k+1] = A[i];
				} else {
					if (maxV[k+1] > A[i]) 
						maxV[k+1] = A[i];
				}
				break;
			}
		}
	}

	printf("LIS[i]:\n");
	for (int i=1;i<=NUM;i++)
		printf("%d ", LIS[i]);
	printf("\n");
}