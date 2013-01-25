// MatrixChainOrder.cpp : �������̨Ӧ�ó������ڵ㡣
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

// ��ӡһ�����Ž�
void print_optional_parens(int s[][MATRIX_NUM+1], int i, int j);

// p���Ծ���A1 A2 ... An ������ΪԪ�ص�����
void matrix_chain_order(int p[])
{
	// m��¼����������Ž�
	int m[MATRIX_NUM+1][MATRIX_NUM+1];
	
	// s�������Ž�Ĺ������
	int s[MATRIX_NUM+1][MATRIX_NUM+1];

	// ��ʼ��m�ĶԽ����ϵ�ֵ
	for (int i=1;i<=MATRIX_NUM;i++) {
		m[i][i] = 0;
	}

	// �ݹ鹫ʽ m[i][j] = min(m[i][k] + m[k+1][j] + p[i]*p[k]*p[j])�� i<=k<j

	// ѭ���������m�����������ֵ
	// d��m[i][j]��j-i��ֵ����1��ʼ������MATRIX_NUM-1
	for (int d=1;d<MATRIX_NUM;d++) {
		for (int i=1;i<=MATRIX_NUM-d;i++) {
			int j = i+d;
			int min = INT32_MAX;
			for (int k=i;k<j;k++) {
				if ((m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]) < min) {
					min = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];
					// ��¼���ε�ѡ��
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

	// m[1][MATRIX_NUM]�����Ž�
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
	
	// ����һ�����Ž�
	print_optional_parens(s, 1, MATRIX_NUM);
	printf("\n");
}

// ��ӡ�� ((A1(A2A3))((A4A5)A6))����ʽ
void print_optional_parens(int s[][MATRIX_NUM+1], int i, int j)
{
	// ֹͣ����
	if (i==j) {
		printf("A%d", i);
	} else {
		printf("( ");
		print_optional_parens(s, i, s[i][j]);
		print_optional_parens(s, s[i][j]+1, j);
		printf(" )");
	}
}