#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"TreeNode.h"


using namespace std;


//��ʾһ���󲿶���ͬ������������ȡ������
//һ����˵�ж��ٸ��󲿷��ս������Ҫ�����ٿ��������������ӵ���ȡ
class LeftCommonFactorTree {
private:
	TreeNode* treeRoot;//�������ĸ�

	Symbol treeLeft;//��ʾ�������������һ����

	const static string TREE_ROOT_LABEL;//���ĸ��ڵ�ķ��Ŵ�������Ϊ#��


	//�ж��Ƿ����ĸ�
	bool isTreeRoot(TreeNode* treeNode) {
		return treeNode == treeRoot;
	}


	//�ݹ��ͷ����ڵ���ڴ�
	void releaseNodeMemory(TreeNode* nodeToRelease) {
		vector<TreeNode*>& childsToRelease = nodeToRelease->getChilds();
		for (auto& childToRelease : childsToRelease) {
			releaseNodeMemory(childToRelease);
		}
		delete nodeToRelease;
	}

public:
	//���캯��
	//��ʼ�����ڵ㣬������
	LeftCommonFactorTree(TreeNode* treeRoot, Symbol& treeLeft) {
		this->treeRoot = treeRoot;
		this->treeLeft = treeLeft;
	}


	//get����
	TreeNode* getTreeRoot() {
		return treeRoot;
	}


	//productions�������ϰ���������S��S'��S''�������󲿵����в���ʽ
	//һ��ʼ�ĵ���Ӧ�ô������ĸ��ڵ�
	Production extractLeftCommonFactor(TreeNode* currentNode, map<Symbol, vector<Production>>& productionsMap) {
		vector<Production> containingProductions = currentNode->getContainingProductions();//��ǰ�ڵ��������Ĳ���ʽ

		//�����ǰ�ڵ��������Ĳ���ʽֻ��һ����˵���ýڵ����µ�������·��ֻ��һ��
		//��������Ҫ��ȡ�����ӵ������ֱ�ӷ��ز���ʽ��������ȡ������
		if (containingProductions.size() == 1) {
			//����Ǹ��ڵ㣬������ӵ�map�����У���Ȼֱ�ӷ���ֵ�ˣ����õ�ʱ����û�н���
			//B->b������ͻᱻ�Ե�
			if (isTreeRoot(currentNode)) {
				productionsMap[treeLeft] = containingProductions;
			}
			return containingProductions[0];
		}

		//�ȵݹ���ý��������Ĺ�������ȡ
		vector<Production> childProductions;//���������ÿ��·����ȡ�����Ӻ�Ľ��
		vector<TreeNode*>& childs = currentNode->getChilds();

		//����ÿ�����ӣ�����������ÿһ��·��
		for (TreeNode* child : childs) {
			//�ݹ���ã������в�ͬ��·��������ͬ�ĺ��ӽ�����ȡ�����Ӻ󷵻���ȡ��Ľ��
			//���������ļ�����
			childProductions.push_back(extractLeftCommonFactor(child, productionsMap));
		}

		//���б��ڵ�Ĺ�������ȡ

		//����Ǹ��ڵ㣬���ô��������ڵ�����в���ʽ��ӵ�����ʽmap�����м���
		//���ڵ�Ĳ���ʽͬʱ��������Щû�й����ӵ�
		if (isTreeRoot(currentNode)) {
			productionsMap[treeLeft].insert(productionsMap[treeLeft].end(), childProductions.begin(), childProductions.end());
			return Production();
		}

		//���������ȡ�����Ӻ�ֻ��һ��·����Ҳ����ֻ��һ������ʽ����ֱ�ӷ���
		if (childProductions.size() == 1) {
			return childProductions[0];
		}

		//�����������Ҫ�ϲ�·������ȡ������
		//���ȴ����µķ�����Ϊ��ȡ�����Ӻ���󲿣���S������S'
		Symbol copyLeft = Alphabet::addCopy(treeLeft);//��ȡ�����Ӻ������

		//S->ab|acҪ���S->aS'��S'->b|c
		//������S'->b|c
		//����ÿһ������ʽ����ȡ��ǰλ��֮����ַ�����
		//��һ��ѭ����ȡb���ڶ���ѭ����ȡc
		//Ȼ������Ҳ������У����������ʽmap������
		vector<Symbol> prefixSymbols = currentNode->getPrefixSymbols();//����ڲ���ʽ֮�󣬵�ǰ����֮ǰ�Ĺ�ͬ����
		for (Production& childProduction : childProductions) {
			vector<Symbol> right = childProduction.getRight();//��ǰ����ʽ���Ҳ�
			vector<Symbol> newRight;//�µ��Ҳ�����S'��ĳһ���Ҳ�

			//����prefixSymbols�Ĵ�С���õ���ǰ�ڵ�ǰ���ж��ٷ���
			//���Եõ���ȡ��ʼ���±꣬��ȡ�����
			for (int i = prefixSymbols.size(); i < right.size(); i++) {
				//[]����������ع�
				newRight.push_back(right[i]);
			}
			//����µ��Ҳ�Ϊ�գ���Ϊ�����epsilon
			//����S->aA|a�������
			if (newRight.size() == 0) {
				newRight.push_back(Alphabet::getEpsilonSymbol());
			}

			//�������Ҳ�֮��Ϳ��Դ�������ʽ���������ʽmap������
			if (productionsMap.find(copyLeft) == productionsMap.end()) {
				productionsMap[copyLeft] = { Production(copyLeft,newRight) };
			}
			else {
				productionsMap[copyLeft].push_back(Production(copyLeft, newRight));
			}
		}

		//����S->aS'
		vector<Symbol> newTreeRight(prefixSymbols);//����S�����Ҳ�
		//��S'��ӵ�������Ҳ���
		newTreeRight.push_back(copyLeft);
		return Production(treeLeft, newTreeRight);
	}


	//���������һ������ʽ
	void addProduction(Production productionToAdd) {
		TreeNode* parentNode = treeRoot;//�Ӹ��ڵ㿪ʼ
		treeRoot->addProduction(productionToAdd);//���ڵ�������ʽ
		vector<Symbol> rightToAdd = productionToAdd.getRight();//Ҫ��ӵ��Ҳ�

		//����ÿһ�����źͺ��ӣ����бȽ��ж��Ƿ��нڵ��Ӧ�����û������ӷ��ţ���󽫲���ʽ�ӵ����ŵĳ�Ա��
		for (int i = 0; i <= rightToAdd.size(); i++) {
			Symbol symbolToAdd;
			//�ڲ���ʽ��������epsilon
			//S->aA|a�������������
			if (i == rightToAdd.size()) {
				symbolToAdd = Alphabet::getEpsilonSymbol();
			}
			else {
				symbolToAdd = rightToAdd[i];
			}

			vector<TreeNode*>& childs = parentNode->getChilds();//������к��ӽڵ�
			TreeNode* nodeToAdd = nullptr;//���Ŷ�Ӧ�Ľڵ㣬û�ж�Ӧ���򴴽�������ֵ
			for (auto& child : childs) {
				//�ҵ��˷�������Ӧ�Ľڵ㣬��Symbolֵһ��
				//������������==��
				if (child->getNodeSymbol() == symbolToAdd) {
					nodeToAdd = child;
					break;
				}
			}
			//���nodeToAddΪ�գ���û���ҵ����Ŷ�Ӧ�Ľڵ㣬Ҫ���д���
			if (nodeToAdd == nullptr) {
				vector<Symbol> prefixSymbols(parentNode->getPrefixSymbols());
				//����ǰ�ķ��ż��뵽������
				prefixSymbols.push_back(symbolToAdd);
				nodeToAdd = new TreeNode(symbolToAdd, prefixSymbols);
				//���ڵ�ĺ��ӽڵ�����½��Ľڵ�
				parentNode->addChild(nodeToAdd);
			}

			//���ۺ��ӽڵ��Ƿ��½��ģ���Ҫ������ʽ����
			nodeToAdd->addProduction(productionToAdd);
			parentNode = nodeToAdd;//˳������·������
		}
	}


	//����һ�����ĸ��ڵ㣬��̬����
	static TreeNode* createTreeRoot() {
		Symbol treeRootSymbol(TREE_ROOT_LABEL, Symbol::TERMINATOR);
		return new TreeNode(treeRootSymbol);
	}


	//���õݹ麯�����ͷ����Ľڵ��ڴ�
	void releaseMemory() {
		releaseNodeMemory(this->treeRoot);
	}
};


