#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Production.h"


using namespace std;


//����ʽ���Ӧ������
//Ҳ��ʾ���е�һ���ڵ㣬ͨ��childs���ԣ����ӽڵ㹲ͬ����һ������
//���磬S->ac|aab|A����a�Ľڵ�������c��ab
class TreeNode {
private:
	Symbol nodeSymbol;//��ǰ���ڵ��Ӧ�ķ���

	vector<Symbol> prefixSymbols;//��ǰ�ڵ�ǰ��ķ��ż��ϣ�A->abcde����c�ڵ��Ӧ[a,b]

	vector<TreeNode*> childs;//��ǰ���ڵ���ӽڵ㣬Ҳ��ʾ�������

	vector<Production> containingProductions;//��ǰ���ڵ���������в���ʽ����Ϊһ������ʽ��Ӧһ��·�������Ҳ��ʾ�м���·����Ҳ��ʾ�����м�ƬҶ��

public:
	//���캯��
	//��ʼ���ڵ���ţ�ǰ׺���ż���
	TreeNode() :TreeNode(Symbol()) {
	}
	TreeNode(Symbol nodeSymbol, vector<Symbol> prefixSymbols) {
		this->prefixSymbols = prefixSymbols;
		this->nodeSymbol = nodeSymbol;
		this->containingProductions = vector<Production>();
		this->childs = vector<TreeNode*>();

	}
	TreeNode(Symbol nodeSymbol) :TreeNode(nodeSymbol, vector<Symbol>()) {
	}


	//get����
	vector<Production> getContainingProductions() {
		return containingProductions;
	}
	vector<TreeNode*>& getChilds() {
		return childs;
	}
	vector<Symbol> getPrefixSymbols() {
		return prefixSymbols;
	}
	Symbol getNodeSymbol() {
		return nodeSymbol;
	}


	//�򼯺�������Ԫ��
	void addProduction(Production productionToAdd) {
		containingProductions.push_back(productionToAdd);
	}
	//��Ӻ��ӽڵ�
	void addChild(TreeNode* childToAdd) {
		childs.push_back(childToAdd);
	}
};
