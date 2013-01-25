// AssemblyStations.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <assert.h>

#define STATION_NUM 6

void fastest_way(int a1[], int a2[], int t1[], int t2[], int e1, int e2, int x1, int x2);

int _tmain(int argc, _TCHAR* argv[])
{
	int a1[] = {0, 7, 9, 3, 4, 8, 4};
	int a2[] = {0, 8, 5, 6, 4, 5, 7};
	int t1[] = {0, 2, 3, 1, 3, 4};
	int t2[] = {0, 2, 1, 2, 2, 1};
	int e1 = 2;
	int e2 = 4;
	int x1 = 3;
	int x2 = 2;

	fastest_way(a1, a2, t1, t2, e1, e2, x1, x2);
	
	return 0;
}

void print_way(int l1[], int l2[], int N, int l);

// 找到最快的装配路径
// a1是装配线1的每个站的装配时间
// a2是装配线2的每个站的装配时间
// t1是装配线1的每个站的转移时间
// t2是装配线1的每个站的转移时间
// e1是上转配线1的时间
// e2是上转配线2的时间
// x1是下转配线1的时间
// x2是下转配线2的时间
void fastest_way(int a1[], int a2[], int t1[], int t2[], int e1, int e2, int x1, int x2)
{
	assert(a1 && a1 && t1 && t2 && e1>=0 && e2>=0 && x1>=0 && x2>=0);

	int f1[STATION_NUM+1]; // 到达装配线1的第i站的最短时间
	int f2[STATION_NUM+1]; // 到达装配线2的第i站的最短时间
	
	// 到达装配线1、2的第1站的时间，可以直接计算出来
	f1[1] = e1 + a1[1];
	f2[1] = e2 + a2[1];
	
	// 这两个数组跟踪最优解的构造过程，最终构造路径
	int l1[STATION_NUM+1];  // 记录装配线1的第i-1站所做的选择（较短时间） 
	int l2[STATION_NUM+1];  // 记录装配线2的第i-1站所做的选择（较短时间） 
	l1[1] = 1;
	l2[1] = 2;

	int N = STATION_NUM;

	// 循环到达计算装配线第i站的时间
	// 递归公式 f1[i] = min(f1[i-1]+a1[i], f2[i-1]+t2[i-1]+a1[i])
	// 递归公式 f2[i] = min(f2[i-1]+a2[i], f1[i-1]+t1[i-1]+a2[i])	
	for (int i=2;i<=N;i++) {
		if ((f1[i-1] + a1[i]) < (f2[i-1] + t2[i-1] + a1[i])) {
			f1[i] = f1[i-1] + a1[i];
			l1[i] = 1;
		} else {
			f1[i] = f2[i-1] + t2[i-1] + a1[i];
			l1[i] = 2;
		}

		if ((f2[i-1] + a2[i]) < (f1[i-1] + t1[i-1] + a2[i])) {
			f2[i] = f2[i-1] + a2[i];
			l2[i] = 2;
		} else {
			f2[i] = f1[i-1] + t1[i-1] + a2[i] ;
			l2[i] = 1;
		}
	}

	int f;	// 最短时间
	int l;  // 用于构造路径
	if ((f1[N] + x1) < (f2[N] + x2)) {
		f = f1[N] + x1;
		l = 1;
	} else {
		f = f2[N] + x2;
		l = 2;
	}

	printf("fastest way: %d\n", f);

	print_way(l1, l2, N, l);
}

void print_way(int l1[], int l2[],int N, int l)
{
	// 递归结束条件
	if (N<1)
		return;

	if (1 == l) {
		print_way(l1, l2, N-1, l1[N]);
		printf("line 1, station %d\n", N);
	} else {
		print_way(l1, l2, N-1, l2[N]);
		printf("line 2, station %d\n", N);
	}
}