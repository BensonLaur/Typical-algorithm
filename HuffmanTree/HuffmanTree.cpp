// HuffmanTree.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//�������������ο� http://www.cnblogs.com/wuyuankun/p/3982216.html

//ʵ�ֹ�����������ͽ���

//��������Դ������ ABCDE 5�ֲ�ͬ�����ݣ�ÿһ�����ݵ�Ȩ�طֱ��� 5 4 3 2 1
//����5������ ��Ӧ�Ĺ���������
//��֪������ 001011110101110010111001000000110100000111001010101010101010000011111100110100101111 ���Ӧ��ԭʼ����


//ԭʼ���ݶ���
typedef struct _RawData
{
	string value;
	int weight;

	string actualData;
}RawData;

//���������붨��
typedef struct _Code
{
	string value;
	string code;
}Code;

//���������ڵ㶨��
typedef struct _Node
{
	bool isleaf;
	string value;
	int weight;

	struct _Node* parent;
	struct _Node* leftChild;
	struct _Node* rightChild;

}Node, *pNode;

//������
class HuffManTree
{
public:
	//��ʼ���������������������������нӿ�ִ�У�
	void InitTree(vector<RawData> vecData)
	{
		vecDataBackup = vecData;

		//���
		if (!m_vecpNode.empty())
		{
			for (auto node : m_vecpNode)
				delete  node;
			m_vecpNode.clear();
		}

		//��ʼ��ÿһ���ڵ�
		for (auto data : vecData)
		{
			pNode p = new Node;
			p->isleaf = true;
			p->value = data.value;
			p->weight = data.weight;
			p->parent = nullptr;
			p->leftChild = nullptr;
			p->rightChild = nullptr;

			m_vecpNode.push_back(p);
		}

		//������������
		while (!HasOnlyOneRoot(m_vecpNode))//ѭ��ֱ��ʣ��һ�����ڵ㣨û��parent��Ϊֹ
		{
			//�����и��ڵ��У��ҳ�2��Ȩ����С�Ľڵ�
			int nMin1, nMin2;
			nMin1 = nMin2 = GetMaxWeight(m_vecpNode);

			vector<pNode>::iterator iterM1, iterM2;
			for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //�õ� nMin1 ��nMin2 ��Ӧ�ĵ�����
			{
				if ((*iter)->parent == nullptr)
				{
					if ((*iter)->weight == nMin1)
					{
						iterM1 = iterM2 = iter;
						break;
					}
				}
			}

			for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //�õ���С�� nMin1, nMin2
			{
				if ((*iter)->parent == nullptr)
				{
					if ((*iter)->weight < nMin2)
					{
						nMin1 = nMin2;
						nMin2 = (*iter)->weight;
						iterM1 = iterM2;
						iterM2 = iter;
					}

				}
			}

			//�����µĽڵ�
			pNode p = new Node;
			p->isleaf = false;
			p->value = ""; //û��ֵ
			p->weight = nMin1 + nMin2;
			p->parent = nullptr;
			p->leftChild = *iterM1;
			p->rightChild = *iterM2;

			//����2���ӽڵ�
			(*iterM1)->parent = p;
			(*iterM2)->parent = p;

			//�����µĸ��ڵ�
			m_vecpNode.push_back(p);
		}
	}

	//�õ����������� ���� m_vecCode�����Ҵ�ӡ���������� Encode ִ�У�
	void GetAndPrintCode()
	{
		//�����������������ֻ���Ҷ�ӽڵ�

		auto iterRoot = m_vecpNode.begin();
		for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //�õ����ڵ�
		{
			if ((*iter)->parent == nullptr)
			{
				iterRoot = iter;
			}
		}

		//m_vecpNode

		m_vecCode.clear();
		LeftOrderTravel(*iterRoot);
	}

	//���ַ����б���
	void Encode(string str)
	{
		cout << "Դ���ݣ�" << str << endl;

		string encoded = "";
		for (auto ch : str)
		{
			encoded += GetCode(string(1,ch));
		}
		cout << "�����"<< encoded << endl;
	}

	//���Ѿ�������ַ����н���
	void Decode(string strToDecode)
	{
		cout << "�������ݣ�" << strToDecode << endl;
		string decoded = "";

		auto iterRoot = m_vecpNode.begin();
		for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //�õ����ڵ�
		{
			if ((*iter)->parent == nullptr)
			{
				iterRoot = iter;
			}
		}

		string easyStr = "";
		string tmpStr;
		bool bMatch;
		int nIndex = 0;
		while (nIndex < strToDecode.size()) //���������ÿһ��Ԫ��
		{
			tmpStr = "";
			bMatch = false;
			pNode pN= *iterRoot;
			while (nIndex < strToDecode.size())
			{
				if (strToDecode.at(nIndex) == '0')
				{
					pN = pN->leftChild;

					tmpStr += "0";
					if (pN->isleaf)
					{
						decoded += pN->value;  //�õ���Ӧ��ԭʼ����
						bMatch = true;
						easyStr += tmpStr + " ";
						break;
					}
				}
				else
				{
					pN = pN->rightChild;

					tmpStr += "1";
					if (pN->isleaf)
					{
						decoded += pN->value;//�õ���Ӧ��ԭʼ����
						bMatch = true;
						easyStr += tmpStr + " ";
						break;
					}
				}

				nIndex++;
			}
			nIndex++;
		}
		
		if (!bMatch)
		{
			decoded += " [" + tmpStr + "]";
			easyStr += " [" + tmpStr + "]";
		}

		cout << "�׶����ݣ�" << easyStr << endl;
		cout << "ԭʼ���ݣ�" << decoded << endl;


		cout << "ʵ�����ݣ�" << endl;;
		string actualStr = "";
		for (auto ch : decoded)
		{
			actualStr += GetActualData(string(1, ch));
		}
		cout << actualStr << endl;
	}

private:

	// ��ǰ�������� �Ƿ�ֻ��1�� ���ڵ�
	bool HasOnlyOneRoot(const vector<pNode> &vecpNode)
	{
		int nCount = 0;

		for (auto pNod : vecpNode)
		{
			if (pNod->parent == NULL)
			{
				nCount++;
				if (nCount >= 2)
					return false;
			}
		}
		return true;
	}

	//������� ���ڵ��� Ȩ������ֵ
	int GetMaxWeight(vector<pNode> &vecpNode)
	{
		int nMaxWeight = 0;
		bool bFirst = true;

		for (auto pNod : vecpNode)
		{
			if (pNod->parent == NULL)
			{
				if (bFirst)
				{
					bFirst = !bFirst;
					nMaxWeight = pNod->weight;
				}
				else
					nMaxWeight = nMaxWeight >= pNod->weight ? nMaxWeight : pNod->weight;
			}
		}

		return nMaxWeight;
	}

	//��������������������������������
	void LeftOrderTravel(pNode pN, string parentCode = "")
	{
		if (pN == nullptr)
			return;

		LeftOrderTravel(pN->leftChild, parentCode + "0");

		if (pN->isleaf){
			cout << pN->value << "\t" << parentCode << endl;
			m_vecCode.push_back({ pN->value, parentCode });
		}

		LeftOrderTravel(pN->rightChild, parentCode + "1");
	}

	//��ö�Ӧ�ı���
	string GetCode(string value)
	{
		for (auto code : m_vecCode)
		{
			if (code.value == value)
				return code.code;
		}

		return "";
	}

	//���ʵ�ʵ�����
	string GetActualData(string value)
	{
		string data = "";
		for (auto d : vecDataBackup)
		{
			if (d.value == value)
				data = d.actualData;
		}

		return data;
	}

private:
	vector<pNode> m_vecpNode;
	vector<Code> m_vecCode;
	vector<RawData> vecDataBackup;
};

//�������
int _tmain(int argc, _TCHAR* argv[])
{
	HuffManTree tree;

	//�õ�ԭʼ������
	vector<RawData> vecData;
	vecData.push_back({ "A", 5 ,"aaaaaaaaaaaaaaaaaaaaaaaaa"});
	vecData.push_back({ "B", 4 ,"bbbbbbbbbbbbbbbbbbbb"});
	vecData.push_back({ "C", 3 ,"ccccccccccccccc"});
	vecData.push_back({ "D", 2 ,"dd"});
	vecData.push_back({ "E", 1 ,"e"});

	tree.InitTree(vecData);

	cout << "-------------------------------------------------" << endl;
	tree.GetAndPrintCode();

	cout << "-------------------------------------------------" << endl;
	tree.Encode("ABDECAABBD");

	cout << "-------------------------------------------------" << endl;
	tree.Decode("0010111101011100101110010000001101000001110010101010101010100000111111001101001011111");
	cout << "-------------------------------------------------" << endl;

	system("pause");

	return 0;
}

