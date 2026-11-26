#include <stdio.h>
#define N 100
int main()
{
	int i, j, n, s, m, a[N], b[N][N];
	printf("\n Enter the leftidentity m:\n");
	scanf_s("%d", &m);
	do
	{
		printf("\n Enter the data n: ");
		scanf_s("%d", &n);
	} while (n <= 0 || n > N);
	printf("\n Enter the elements of set A:\n");
	for (i = 0; i < n; i++)
		scanf_s("%d", &a[i]);
	printf("\n Enter the elements of matrix B:\n");
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) scanf_s("%d", &b[i][j]);
	printf("\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			if (b[j][i] == m) {
				s = 0;
				break;
			}
			else s = 1;
	}
	if (s == 0)
		printf("Yes!\n");
	else
		printf("No!\n");
}