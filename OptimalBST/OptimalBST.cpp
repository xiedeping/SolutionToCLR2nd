// OptimalBST.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define NUM 5
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

void optimal_bst(int p[], int q[])
{
	// 递归公式
	//          -   q
	//          |
	// e[i,j] = -
	//          |
	//          -
	int e[NUM+1][NUM+1];
	int w[NUM+1][NUM+1];

	for (int i=1;i<=NUM;i++) {
		e[i][i-1] = q[i-1];
		w[i][i-1] = q[i-1];
	}

	for (int l=1;l<=NUM;l++) {
		for (int i=1;i<=NUM-l+1;i++) {
			int j = i+l-1;
			w[i][j] = w[i][j-1] + p[j]+ q[j];
			int min = INT32_MAX;
			for (int k=i;k<=j;k++) {
				if ((e[i][k-1] + e[k+1][j] + w[i][j]) < min)
					min = e[i][k-1] + e[k+1][j] + w[i][j];
			}
			e[i][j] = min;
		}
	}
}