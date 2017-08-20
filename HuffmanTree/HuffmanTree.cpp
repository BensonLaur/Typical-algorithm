// HuffmanTree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//哈夫曼编码概念参考 http://www.cnblogs.com/wuyuankun/p/3982216.html

//实现哈夫曼树编码和解码

//现有数据源，包含 ABCDE 5种不同的数据，每一种数据的权重分别是 5 4 3 2 1
//求这5种数据 对应的哈夫曼编码
//已知编码结果 001011110101110010111001000000110100000111001010101010101010000011111100110100101111 求对应的原始数据


//原始数据定义
typedef struct _RawData
{
	string value;
	int weight;

	string actualData;
}RawData;

//哈夫曼编码定义
typedef struct _Code
{
	string value;
	string code;
}Code;

//哈夫曼树节点定义
typedef struct _Node
{
	bool isleaf;
	string value;
	int weight;

	struct _Node* parent;
	struct _Node* leftChild;
	struct _Node* rightChild;

}Node, *pNode;

//哈夫曼
class HuffManTree
{
public:
	//初始化哈夫曼树（必须先于其他所有接口执行）
	void InitTree(vector<RawData> vecData)
	{
		vecDataBackup = vecData;

		//清空
		if (!m_vecpNode.empty())
		{
			for (auto node : m_vecpNode)
				delete  node;
			m_vecpNode.clear();
		}

		//初始化每一个节点
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

		//构建哈夫曼树
		while (!HasOnlyOneRoot(m_vecpNode))//循环直到剩下一个根节点（没有parent）为止
		{
			//在所有根节点中，找出2个权重最小的节点
			int nMin1, nMin2;
			nMin1 = nMin2 = GetMaxWeight(m_vecpNode);

			vector<pNode>::iterator iterM1, iterM2;
			for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //得到 nMin1 ，nMin2 对应的迭代器
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

			for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //得到最小的 nMin1, nMin2
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

			//构建新的节点
			pNode p = new Node;
			p->isleaf = false;
			p->value = ""; //没有值
			p->weight = nMin1 + nMin2;
			p->parent = nullptr;
			p->leftChild = *iterM1;
			p->rightChild = *iterM2;

			//更新2个子节点
			(*iterM1)->parent = p;
			(*iterM2)->parent = p;

			//加入新的根节点
			m_vecpNode.push_back(p);
		}
	}

	//得到哈夫曼编码 存于 m_vecCode，并且打印（必须先于 Encode 执行）
	void GetAndPrintCode()
	{
		//左序遍历哈夫曼树，只输出叶子节点

		auto iterRoot = m_vecpNode.begin();
		for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //得到根节点
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

	//对字符进行编码
	void Encode(string str)
	{
		cout << "源数据：" << str << endl;

		string encoded = "";
		for (auto ch : str)
		{
			encoded += GetCode(string(1,ch));
		}
		cout << "编码后："<< encoded << endl;
	}

	//对已经编码的字符进行解码
	void Decode(string strToDecode)
	{
		cout << "编码数据：" << strToDecode << endl;
		string decoded = "";

		auto iterRoot = m_vecpNode.begin();
		for (auto iter = m_vecpNode.begin(); iter != m_vecpNode.end(); iter++) //得到根节点
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
		while (nIndex < strToDecode.size()) //遍历编码的每一个元素
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
						decoded += pN->value;  //得到对应的原始数据
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
						decoded += pN->value;//得到对应的原始数据
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

		cout << "易读数据：" << easyStr << endl;
		cout << "原始数据：" << decoded << endl;


		cout << "实际数据：" << endl;;
		string actualStr = "";
		for (auto ch : decoded)
		{
			actualStr += GetActualData(string(1, ch));
		}
		cout << actualStr << endl;
	}

private:

	// 当前哈夫曼树 是否只有1个 根节点
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

	//获得所有 根节点中 权重最大的值
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

	//左序遍历哈夫曼树，以输出哈夫曼编码
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

	//获得对应的编码
	string GetCode(string value)
	{
		for (auto code : m_vecCode)
		{
			if (code.value == value)
				return code.code;
		}

		return "";
	}

	//获得实际的数据
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

//测试入口
int _tmain(int argc, _TCHAR* argv[])
{
	HuffManTree tree;

	//得到原始的数据
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

