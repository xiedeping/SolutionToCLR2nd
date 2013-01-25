// AssemblyStations.cpp : �������̨Ӧ�ó������ڵ㡣
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

// �ҵ�����װ��·��
// a1��װ����1��ÿ��վ��װ��ʱ��
// a2��װ����2��ÿ��վ��װ��ʱ��
// t1��װ����1��ÿ��վ��ת��ʱ��
// t2��װ����1��ÿ��վ��ת��ʱ��
// e1����ת����1��ʱ��
// e2����ת����2��ʱ��
// x1����ת����1��ʱ��
// x2����ת����2��ʱ��
void fastest_way(int a1[], int a2[], int t1[], int t2[], int e1, int e2, int x1, int x2)
{
	assert(a1 && a1 && t1 && t2 && e1>=0 && e2>=0 && x1>=0 && x2>=0);

	int f1[STATION_NUM+1]; // ����װ����1�ĵ�iվ�����ʱ��
	int f2[STATION_NUM+1]; // ����װ����2�ĵ�iվ�����ʱ��
	
	// ����װ����1��2�ĵ�1վ��ʱ�䣬����ֱ�Ӽ������
	f1[1] = e1 + a1[1];
	f2[1] = e2 + a2[1];
	
	// ����������������Ž�Ĺ�����̣����չ���·��
	int l1[STATION_NUM+1];  // ��¼װ����1�ĵ�i-1վ������ѡ�񣨽϶�ʱ�䣩 
	int l2[STATION_NUM+1];  // ��¼װ����2�ĵ�i-1վ������ѡ�񣨽϶�ʱ�䣩 
	l1[1] = 1;
	l2[1] = 2;

	int N = STATION_NUM;

	// ѭ���������װ���ߵ�iվ��ʱ��
	// �ݹ鹫ʽ f1[i] = min(f1[i-1]+a1[i], f2[i-1]+t2[i-1]+a1[i])
	// �ݹ鹫ʽ f2[i] = min(f2[i-1]+a2[i], f1[i-1]+t1[i-1]+a2[i])	
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

	int f;	// ���ʱ��
	int l;  // ���ڹ���·��
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
	// �ݹ��������
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