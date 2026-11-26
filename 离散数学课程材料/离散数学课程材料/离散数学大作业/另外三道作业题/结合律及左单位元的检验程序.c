#include <stdio.h>
#define N 100
int f(int m, int n, int b[N][N])
{
	return b[m][n];
}
int g(int m, int n, int a[N], int b[N][N])
{
	int k, t = -1;
	for (k = 0; k < N; k++)
		if (a[k] == f(m, n, b))
		{
			t = k; break;
		}
	return t;
}
getdata(int a[N], int b[N][N], int n)
{
	int i, j;
	i = 0; j = 0;
	printf("\n Enter the elements of set A:\n");
	for (i = 0; i < n; i++)
		scanf_s("%d", &a[i]);
	printf("\n Enter the elements of matrix B:\n");
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) scanf_s("%d", &b[i][j]);
	printf("\n");
	return;
}
void check(int a[N], int b[N][N], int n)
{
	int i, j, k, s = 0;
	for (i = 0; i < n; i++)
		if (s == 1) break;
		else
			for (j = 0; j < n; j++)
				if (s == 1) break;
				else
					for (k = 0; k < n; k++)
						if (f(g(i, j, a, b), k, b) != f(i, g(j, k, a, b), b))
						{
							s = 1;
							printf("NO!\n");
							printf("%2c%2c%2c", a[i], a[j], a[k]);
							printf("\n");
							break;
						}
	if (s == 0) printf("YES!\n");
	return;
}
main()
{
	int i, j, n, t = 0, a[N], b[N][N];
	do
	{
		printf("\n Enter the data n: ");
		scanf_s("%d", &n);
	} while (n <= 0 || n > N);
	getdata(a, b, n);
	check(a, b, n);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			if (b[i][j] == a[j])
			{
				printf("%d", a[i]);
				t++;
			}
	}
	printf("%4d\n", t);
	return;
}