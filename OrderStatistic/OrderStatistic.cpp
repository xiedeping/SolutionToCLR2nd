// OrderStatistic.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "extremum.h"
#include "randomized_select.h"
#include "median.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// 测试 extremum.h
	//int A[] = {-1,4,6,1,2,5,9,8,0,3,7};
	//int n = sizeof(A)/sizeof(A[0]) - 1;  // A[0]不被使用
	//int min,max;
	//min = minimum(A, n);
	//max = maximum(A, n);
	//printf("min: %d max: %d\n", min, max);
	//minmax(A, n, min, max);
	//printf("min: %d max: %d\n", min, max);

	// 测试randomized_select.h
	//srand ( time(NULL) );
	//int A[] = {-1,4,6,1,2,5,9,8,0,3,7};
	//int n = sizeof(A)/sizeof(A[0]) - 1;  // A[0]不被使用
	//for (int i=1;i<=n;i++) {
	//	printf("第%d小的元素: %d\n", i, randomized_select(A, n, i));
	//}

	// 测试median.h
	srand ( time(NULL) );
	int A[] = {-1,1,2,3,4};
	int n = sizeof(A)/sizeof(A[0]) - 1;  // A[0]不被使用
	int m = median(A, n);
	printf("median of A: %d\n", m);
	int B[] = {-1,3,1,7,8};
	m = median_two_array(A, B, n);
	printf("median of A and B: %d\n", m);
	return 0;
}

