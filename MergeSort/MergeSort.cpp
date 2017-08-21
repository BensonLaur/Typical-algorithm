// MergeSort.cpp : �������̨Ӧ�ó������ڵ㡣
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


//�����鲢
//��r[i��m]��r[m +1 ��n]�鲢����������rf[i��n]  
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
			Merge(q, rf, i, i + s - 1, i + len - 1); //�Եȳ��������ӱ�ϲ�  
			i = i + len;
		}
		if (i + s < lenght){
			Merge(q, rf, i, i + s - 1, lenght - 1); //�Բ��ȳ��������ӱ�ϲ�  
		}
		tmp = q; q = rf; rf = tmp; //����q,rf���Ա�֤��һ�˹鲢ʱ���Դ�q �鲢��rf  
	}
}


//��·�鲢�ݹ�
void Merge(int *nArray,  int left, int right)
{
	if (left == right)
		return;

	int middle = (left + right) / 2;
	Merge(nArray, left, middle);		//������������
	Merge(nArray, middle + 1, right);	//������������

	//��2�������н��в�������
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
	cout << "�����" << endl;
	print(a, 10);

	cout << "------------------------------" << endl;
	print(c, 10);
	MergeSort2(c, 10);  //2·�ݹ�����
	cout << "�����" << endl;
	print(c, 10);

	system("pause");
	return 0;
}

