#include <stdio.h>
#include <iostream>

void qsort(int* arr, int l, int r)
{
	if (l >= r)
		return;

	int tmp = arr[l];
	int first = l;
	int last = r;

	while (first < last)
	{
		while (first<last && arr[last] > tmp)
		{
			last--;
		}
		arr[first] = arr[last];

		while (first<last && arr[first] < tmp)
		{
			first++;
		}
		arr[last] = arr[first];
	}

	arr[first] = tmp;
	qsort(arr, l, first-1);
	qsort(arr, first+1, r);
}

void main()
{

	int a[] = {57, 68, 59, 52, 72, 28, 96, 33, 24};

	qsort(a, 0, sizeof(a) / sizeof(a[0]) - 1);/*这里原文第三个参数要减1否则内存越界*/

	for(int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		printf("%d ", a[i]);
	}

	system("pause");
}

