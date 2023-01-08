#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"TreeNode.h"


using namespace std;


//表示一棵左部都相同的树，用来提取左公因子
//一般来说有多少个左部非终结符，就要建多少棵树，进行左公因子的提取
class LeftCommonFactorTree {
private:
	TreeNode* treeRoot;//保存树的根

	Symbol treeLeft;//表示这课子树属于哪一个左部

	const static string TREE_ROOT_LABEL;//树的根节点的符号代表，可以为#号


	//判断是否树的根
	bool isTreeRoot(TreeNode* treeNode) {
		return treeNode == treeRoot;
	}


	//递归释放树节点的内存
	void releaseNodeMemory(TreeNode* nodeToRelease) {
		vector<TreeNode*>& childsToRelease = nodeToRelease->getChilds();
		for (auto& childToRelease : childsToRelease) {
			releaseNodeMemory(childToRelease);
		}
		delete nodeToRelease;
	}

public:
	//构造函数
	//初始化根节点，树的左部
	LeftCommonFactorTree(TreeNode* treeRoot, Symbol& treeLeft) {
		this->treeRoot = treeRoot;
		this->treeLeft = treeLeft;
	}


	//get函数
	TreeNode* getTreeRoot() {
		return treeRoot;
	}


	//productions参数集合包括了类似S，S'，S''这样的左部的所有产生式
	//一开始的调用应该传入树的根节点
	Production extractLeftCommonFactor(TreeNode* currentNode, map<Symbol, vector<Production>>& productionsMap) {
		vector<Production> containingProductions = currentNode->getContainingProductions();//当前节点所包含的产生式

		//如果当前节点所包含的产生式只有一个，说明该节点以下的子树的路径只有一条
		//不会有需要提取公因子的情况，直接返回产生式，不用提取公因子
		if (containingProductions.size() == 1) {
			//如果是根节点，还得添加到map集合中，不然直接返回值了，调用的时候又没有接收
			//B->b的情况就会被吃掉
			if (isTreeRoot(currentNode)) {
				productionsMap[treeLeft] = containingProductions;
			}
			return containingProductions[0];
		}

		//先递归调用进行子树的公因子提取
		vector<Production> childProductions;//存放子树中每条路径提取公因子后的结果
		vector<TreeNode*>& childs = currentNode->getChilds();

		//遍历每个孩子，即走子树中每一条路径
		for (TreeNode* child : childs) {
			//递归调用，子树中不同的路径，即不同的孩子进行提取公因子后返回提取后的结果
			//放入整个的集合中
			childProductions.push_back(extractLeftCommonFactor(child, productionsMap));
		}

		//进行本节点的公因子提取

		//如果是根节点，则不用处理，将根节点的所有产生式添加到产生式map集合中即可
		//根节点的产生式同时包含了那些没有公因子的
		if (isTreeRoot(currentNode)) {
			productionsMap[treeLeft].insert(productionsMap[treeLeft].end(), childProductions.begin(), childProductions.end());
			return Production();
		}

		//如果子树提取公因子后只有一条路径，也就是只有一个产生式，则直接返回
		if (childProductions.size() == 1) {
			return childProductions[0];
		}

		//其他情况就需要合并路径，提取公因子
		//首先创建新的符号作为提取公因子后的左部，即S，创建S'
		Symbol copyLeft = Alphabet::addCopy(treeLeft);//提取公因子后的新左部

		//S->ab|ac要变成S->aS'，S'->b|c
		//创建出S'->b|c
		//遍历每一个产生式，截取当前位置之后的字符出来
		//第一次循环截取b，第二次循环截取c
		//然后放入右部集合中，最后放入产生式map集合中
		vector<Symbol> prefixSymbols = currentNode->getPrefixSymbols();//获得在产生式之后，当前符号之前的共同符号
		for (Production& childProduction : childProductions) {
			vector<Symbol> right = childProduction.getRight();//当前产生式的右部
			vector<Symbol> newRight;//新的右部，即S'的某一个右部

			//根据prefixSymbols的大小，得到当前节点前面有多少符号
			//可以得到截取开始的下标，截取到最后
			for (int i = prefixSymbols.size(); i < right.size(); i++) {
				//[]运算符已重载过
				newRight.push_back(right[i]);
			}
			//如果新的右部为空，则为其加上epsilon
			//处理S->aA|a这种情况
			if (newRight.size() == 0) {
				newRight.push_back(Alphabet::getEpsilonSymbol());
			}

			//有了新右部之后就可以创建产生式，放入产生式map集合中
			if (productionsMap.find(copyLeft) == productionsMap.end()) {
				productionsMap[copyLeft] = { Production(copyLeft,newRight) };
			}
			else {
				productionsMap[copyLeft].push_back(Production(copyLeft, newRight));
			}
		}

		//创建S->aS'
		vector<Symbol> newTreeRight(prefixSymbols);//创建S的新右部
		//将S'添加到这个新右部中
		newTreeRight.push_back(copyLeft);
		return Production(treeLeft, newTreeRight);
	}


	//向树中添加一个产生式
	void addProduction(Production productionToAdd) {
		TreeNode* parentNode = treeRoot;//从根节点开始
		treeRoot->addProduction(productionToAdd);//根节点加入产生式
		vector<Symbol> rightToAdd = productionToAdd.getRight();//要添加的右部

		//遍历每一个符号和孩子，进行比较判断是否有节点对应，如果没有则添加符号，最后将产生式加到符号的成员中
		for (int i = 0; i <= rightToAdd.size(); i++) {
			Symbol symbolToAdd;
			//在产生式的最后加入epsilon
			//S->aA|a，处理这种情况
			if (i == rightToAdd.size()) {
				symbolToAdd = Alphabet::getEpsilonSymbol();
			}
			else {
				symbolToAdd = rightToAdd[i];
			}

			vector<TreeNode*>& childs = parentNode->getChilds();//获得所有孩子节点
			TreeNode* nodeToAdd = nullptr;//符号对应的节点，没有对应的则创建，有则赋值
			for (auto& child : childs) {
				//找到了符号所对应的节点，即Symbol值一样
				//这里用了重载==号
				if (child->getNodeSymbol() == symbolToAdd) {
					nodeToAdd = child;
					break;
				}
			}
			//如果nodeToAdd为空，则没有找到符号对应的节点，要进行创建
			if (nodeToAdd == nullptr) {
				vector<Symbol> prefixSymbols(parentNode->getPrefixSymbols());
				//将当前的符号加入到集合中
				prefixSymbols.push_back(symbolToAdd);
				nodeToAdd = new TreeNode(symbolToAdd, prefixSymbols);
				//父节点的孩子节点加入新建的节点
				parentNode->addChild(nodeToAdd);
			}

			//无论孩子节点是否新建的，都要将产生式插入
			nodeToAdd->addProduction(productionToAdd);
			parentNode = nodeToAdd;//顺着树的路径往下
		}
	}


	//创建一个树的根节点，静态函数
	static TreeNode* createTreeRoot() {
		Symbol treeRootSymbol(TREE_ROOT_LABEL, Symbol::TERMINATOR);
		return new TreeNode(treeRootSymbol);
	}


	//调用递归函数，释放树的节点内存
	void releaseMemory() {
		releaseNodeMemory(this->treeRoot);
	}
};


