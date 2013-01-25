// MatrixChainOrder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define MATRIX_NUM 6

void matrix_chain_order(int p[]);

int _tmain(int argc, _TCHAR* argv[])
{
	//int p[] = {30, 35, 15, 5, 10, 20, 25};
	int p[] = {5, 10, 3, 12, 5, 50, 6};
	matrix_chain_order(p);
	return 0;
}

// 打印一个最优解
void print_optional_parens(int s[][MATRIX_NUM+1], int i, int j);

// p是以矩阵A1 A2 ... An 的行数为元素的数组
void matrix_chain_order(int p[])
{
	// m记录子问题的最优解
	int m[MATRIX_NUM+1][MATRIX_NUM+1];
	
	// s跟踪最优解的构造过程
	int s[MATRIX_NUM+1][MATRIX_NUM+1];

	// 初始化m的对角线上的值
	for (int i=1;i<=MATRIX_NUM;i++) {
		m[i][i] = 0;
	}

	// 递归公式 m[i][j] = min(m[i][k] + m[k+1][j] + p[i]*p[k]*p[j])， i<=k<j

	// 循环计算矩阵m的右上区域的值
	// d是m[i][j]中j-i的值，从1开始递增到MATRIX_NUM-1
	for (int d=1;d<MATRIX_NUM;d++) {
		for (int i=1;i<=MATRIX_NUM-d;i++) {
			int j = i+d;
			int min = INT32_MAX;
			for (int k=i;k<j;k++) {
				if ((m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]) < min) {
					min = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];
					// 记录本次的选择
					s[i][j] = k;
				}
			}
			m[i][j] = min;
		}
	}

	printf("m[i][j]:\n");
	for (int i=1;i<=MATRIX_NUM;i++) {
		for (int j=1;j<i;j++) {
			printf("          ");
		}
		for (int j=i;j<=MATRIX_NUM;j++) {
			printf("%6d    ", m[i][j]);
		}
		printf("\n");
	}

	// m[1][MATRIX_NUM]即最优解
	printf("minimum: %d\n", m[1][MATRIX_NUM]);

	printf("s[i][j]:\n");
	for (int i=1;i<=MATRIX_NUM;i++) {
		for (int j=1;j<=i;j++) {
			printf("          ");
		}
		for (int j=i+1;j<=MATRIX_NUM;j++) {
			printf("%6d    ", s[i][j]);
		}
		printf("\n");
	}
	
	// 构造一个最优解
	print_optional_parens(s, 1, MATRIX_NUM);
	printf("\n");
}

// 打印出 ((A1(A2A3))((A4A5)A6))的形式
void print_optional_parens(int s[][MATRIX_NUM+1], int i, int j)
{
	// 停止条件
	if (i==j) {
		printf("A%d", i);
	} else {
		printf("( ");
		print_optional_parens(s, i, s[i][j]);
		print_optional_parens(s, s[i][j]+1, j);
		printf(" )");
	}
}