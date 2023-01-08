#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Production.h"


using namespace std;


//产生式组对应的子树
//也表示树中的一个节点，通过childs属性，和子节点共同构成一棵子树
//例如，S->ac|aab|A，则a的节点下面有c和ab
class TreeNode {
private:
	Symbol nodeSymbol;//当前树节点对应的符号

	vector<Symbol> prefixSymbols;//当前节点前面的符号集合，A->abcde，则c节点对应[a,b]

	vector<TreeNode*> childs;//当前树节点的子节点，也表示这棵子树

	vector<Production> containingProductions;//当前树节点包含的所有产生式，因为一个产生式对应一个路径，因此也表示有几条路径，也表示底下有几片叶子

public:
	//构造函数
	//初始化节点符号，前缀符号集合
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


	//get函数
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


	//向集合中增加元素
	void addProduction(Production productionToAdd) {
		containingProductions.push_back(productionToAdd);
	}
	//添加孩子节点
	void addChild(TreeNode* childToAdd) {
		childs.push_back(childToAdd);
	}
};
