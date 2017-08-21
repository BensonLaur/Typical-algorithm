// MergeSort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

void print(int a[], int n){
	for (int j = 0; j<n; j++){
		cout << a[j] << "  ";
	}
	cout << endl;
}


//迭代归并
//将r[i…m]和r[m +1 …n]归并到辅助数组rf[i…n]  
void Merge(int *r, int *rf, int i, int m, int n)
{
	int j, k;
	for (j = m + 1, k = i; i <= m && j <= n; ++k){
		if (r[j] < r[i]) rf[k] = r[j++];
		else rf[k] = r[i++];
	}
	while (i <= m)  rf[k++] = r[i++];
	while (j <= n)  rf[k++] = r[j++];
	print(rf, n + 1);
}

void MergeSort(int *r, int *rf, int lenght)
{
	int len = 1;
	int *q = r;
	int *tmp;
	while (len < lenght) {
		int s = len;
		len = 2 * s;
		int i = 0;
		while (i + len <lenght){
			Merge(q, rf, i, i + s - 1, i + len - 1); //对等长的两个子表合并  
			i = i + len;
		}
		if (i + s < lenght){
			Merge(q, rf, i, i + s - 1, lenght - 1); //对不等长的两个子表合并  
		}
		tmp = q; q = rf; rf = tmp; //交换q,rf，以保证下一趟归并时，仍从q 归并到rf  
	}
}


//两路归并递归
void Merge(int *nArray,  int left, int right)
{
	if (left == right)
		return;

	int middle = (left + right) / 2;
	Merge(nArray, left, middle);		//左子序列排序
	Merge(nArray, middle + 1, right);	//右子序列排序

	//对2个子序列进行插入排序
	for (auto i = left + 1; i <= right; i++)
	{
		int j = i - 1;
		int temp = nArray[i];
		for (; j >= 0 && nArray[j] > temp; j--)
			nArray[j + 1] = nArray[j];
		nArray[j + 1] = temp;
	}
}

void MergeSort2(int *nArray, int nLength)
{
	Merge(nArray, 0, nLength - 1);
}


int _tmain(int argc, _TCHAR* argv[])
{
	int a[10] = { 3, 1, 5, 7, 2, 4, 9, 6, 10, 8 };
	int c[10] = { 3, 1, 5, 7, 2, 4, 9, 6, 10, 8 };
	int b[10];

	MergeSort(a, b, 10);
	print(b, 10);
	cout << "结果：" << endl;
	print(a, 10);

	cout << "------------------------------" << endl;
	print(c, 10);
	MergeSort2(c, 10);  //2路递归排序
	cout << "结果：" << endl;
	print(c, 10);

	system("pause");
	return 0;
}

