// LCS.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define X_LEN 7
#define Y_LEN 6

void lcs_length(char X[], char Y[]);
int _tmain(int argc, _TCHAR* argv[])
{
	char X[] = {'\0', 'A', 'B', 'C', 'B', 'D', 'A', 'B'};
	char Y[] = {'\0', 'B', 'D', 'C', 'A', 'B', 'A'};

	lcs_length(X, Y);
	return 0;
}

void print_lcs(char X[], int b[][Y_LEN+1], int m, int n);

// ��������X��Y�������������
void lcs_length(char X[], char Y[])
{

	// �ݹ鹫ʽ��
	//               -   0                                 if m==0 || n==0
	//               |
	// LCS(Xm,Yn) =  -   LCS(Xm-1,Yn-1)                    if xm == yn
	//               |
	//               _   max(LCS(Xm,Yn01), LCS(Xm-1,Yn-1)) if xm != yn

	// ��¼����������Ž�
	int c[X_LEN+1][Y_LEN+1];
	
	// ��¼���Ž�Ĺ������
	int b[X_LEN+1][Y_LEN+1];

	int m,n;
	// ��ʼ�� LCS(0,y)��LCS(x,0)
	for (m=0;m<=X_LEN;m++) {
		c[m][0] = 0;
	}
	for (n=0;n<=Y_LEN;n++) {
		c[0][n] = 0;
	}

	// ѭ������c[m][n]��ÿ��ֵ
	for (m=1;m<=X_LEN;m++) {
		for (n=1;n<=Y_LEN;n++) {
			if (X[m] == Y[n]) {
				c[m][n] = c[m-1][n-1] + 1;
				b[m][n] = 2;
			} else {
				if (c[m-1][n] >= c[m][n-1]) {
					c[m][n] = c[m-1][n];
					b[m][n] = 3;
				} else {
					c[m][n] = c[m][n-1];
					b[m][n] = 1;
				}
			}
		}
	}
	
	printf("c[m][n]\n");
	for (m=1;m<=X_LEN;m++) {
		for (n=1;n<=Y_LEN;n++) {
			printf("%4d  ", c[m][n]);
		}
		printf("\n");
	}

	// c[X_LEN][Y_LEN]�����Ž�
	printf("lcs-length: %d\n", c[X_LEN][Y_LEN]);

	printf("b[m][n]\n");
	for (m=1;m<=X_LEN;m++) {
		for (n=1;n<=Y_LEN;n++) {
			printf("%d  ", b[m][n]);
		}
		printf("\n");
	}

	// ����һ�����Ž�
	printf("lcs: ");
	print_lcs(X, b, X_LEN, Y_LEN);
	printf("\n");
}

void print_lcs(char X[], int b[][Y_LEN+1], int m, int n)
{
	if (0 == m || 0 == n)
		return;

	if (2 == b[m][n]){
		print_lcs(X, b, m-1, n-1);
		printf("%c  ", X[m]);
	} else if (3 == b[m][n]) {
		print_lcs(X, b, m-1, n);
	} else {
		print_lcs(X, b, m, n-1);
	}
}