#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Production.h"
#include"Grammar.h"
#include"LeftCommonFactorTree.h"
#include"Util.h"
#include"LeftRecursionEliminator.h"


using namespace std;


//提取左公因子的静态类
static class LeftCommonFactorExtractor {
private:
	const static int DEFAULT_SUBSTITUTION_TIMES = 3;//默认的代入非终结符产生式进行推导的代入次数

	static map<Symbol, vector<Production>> deducedProductionsMap;//保存代入推导后得到的所有产生式

	static Grammar grammar;//提取左公因子后的文法

	static Grammar& previousGrammar;//上一个功能进行后的文法


	//对某一个产生式的某一个右部进行非终结符的代入，代入1次
	//因为代入后产生式有可能从一个变成多个，所以返回集合
	//返回的集合中可能有重复的产生式
	//假设A->BCD|CB，B->Ef|Fg，C->fg，D->hH|iI
	//对于A->BCD，新集合：A->EfCD，A->FgCD
	//A->EffgD，A->FgfgD
	//根据A->BCD，对新集合中的每一项都添加C的fg或者D的hH|iI
	//D有两项，hH和iI，则每一项都得变成两项，乘2的意思
	//可以不全部代入，只代入A->BaC，B
	static vector<Production> substituteProduction(Production productionToSubstitute) {
		//当前函数参数应该是类似A->BCD这样的

		Symbol left = productionToSubstitute.getLeft();//左部
		vector<Symbol> originalRight = productionToSubstitute.getRight();//记录原来的右部，BCD
		vector<Production> newProductions;//最后一个产生式有可能变成多个，保存为集合

		//A->B……，空集合变成A->Ef|Fg
		//符号如果和左部符号一样，不进行代入
			
		//bool terminatorAppear = false;//是否出现了终结符，终极符后面的非终极符都不代入
		for (auto originalSymbol : originalRight) {
			//如果符号是拷贝符号，不用代入
			if (originalSymbol.isCopySymbol()
				|| originalSymbol.isNonTerminator() == false
				|| originalSymbol == left
				/*|| terminatorAppear*/) {
				//terminatorAppear = true;

				//如果是空的，要新建
				if (newProductions.empty()) {
					newProductions.push_back(Production(left, vector<Symbol>({ originalSymbol })));
					continue;
				}

				//向每个新右部的最后都添加该终结符
				//如果是终结符，创建一个产生式，并插入
				for (auto& newProduction : newProductions) {
					newProduction.addRightSymbol(originalSymbol);
				}
			}
			else {
				//D->hH|iI
				vector<Production> originalProductions = previousGrammar.getProductions(originalSymbol);//获得符号的所有产生式，准备代入
				for (vector<Production>::iterator it = originalProductions.begin(); it != originalProductions.end();) {
					//空产生式不用代入
					if (it->isEpsilon()) {
						it = originalProductions.erase(it);
						continue;
					}

					//如果S->A，A->aA'，则不代入A
					if (it->hasCopySymbol()) {
						it = originalProductions.erase(it);
						continue;
					}

					it++;
				}
				//经过前面的剔除，originalProductions有可能为空
				//所以要复制上面的代码到这里
				//还它这个非终结符
				if (originalProductions.empty()) {
					//如果是空的，要新建
					if (newProductions.empty()) {
						newProductions.push_back(Production(left, vector<Symbol>({ originalSymbol })));
						continue;
					}

					//向每个新右部的最后都添加该终结符
					//如果是终结符，创建一个产生式，并插入
					for (auto& newProduction : newProductions) {
						newProduction.addRightSymbol(originalSymbol);
					}
					continue;

				}

				if (newProductions.empty()) {
					//在插入之前要修改左部
					for (auto& firstProduction : originalProductions) {
						firstProduction.setLeft(left);
					}
					newProductions.insert(newProductions.end(), originalProductions.begin(), originalProductions.end());
					//这个时候newRights里面可能有多个产生式了
					continue;
				}

				//A->EffgD，A->FgfgD
				vector<Production> previousNewRights(newProductions);//保存新右部的内容，准备替换
				//清空之后进行替换
				newProductions.clear();
				//在每一个右部之中，A->EffgD
				for (auto& previousNewRight : previousNewRights) {
					//在每一个产生式之中，D->hH
					for (auto& originalProduction : originalProductions) {
						newProductions.push_back(Production(left, { previousNewRight.getRight(),originalProduction.getRight() }));
					}
				}
			}
		}
		return newProductions;
	}


	//通过代入非终结符对左部的所有产生式进行推导
	//指定代入的深度，即代入的次数，默认是3次
	//S->aA，A->B，B->b，变成S->aB，是代入1次
	//在提取公因子中用到
	static vector<Production> deduceProductions(Symbol left, int substitutionTimes = DEFAULT_SUBSTITUTION_TIMES) {
		vector<Production> productions = previousGrammar.getProductions(left);//获得所有产生式
		vector<Production> substitutions(productions);//代入后的产生式

		int times = 0;
		while (true) {
			//代入次数达到要求退出循环
			if (times == substitutionTimes) {
				break;
			}
			times++;

			vector<Production> previousSubstitutions(substitutions);//上一次代入产生式的结果
			//清空之后进行替换
			substitutions.clear();
			//遍历所有右部
			//对每一个右部进行代入
			for (Production& previousSubstitutioin : previousSubstitutions) {
				vector<Production> substitution = substituteProduction(previousSubstitutioin);
				//插入到最后的结果集中
				substitutions.insert(substitutions.end(), substitution.begin(), substitution.end());
			}
		}

		return substitutions;
	}


	//进行所有产生式的代入推导，然后保存到集合中
	static void deduceAllProductions() {
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();
		for (auto& symbol : allSymbol) {
			//如果是终结符，不用处理
			if (symbol.isNonTerminator() == false) {
				continue;
			}
			deducedProductionsMap[symbol] = deduceProductions(symbol);
		}
	}


	//输出
	static void printAll() {
		cout << "\ndeducedProductionsMap\n";
		Util::printMapValueInLines(deducedProductionsMap);

		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}

public:
	//get函数
	static map<Symbol, vector<Production>>& getDeducedProductionsMap() {
		return deducedProductionsMap;
	}
	static Grammar& getGrammar() {
		return grammar;
	}


	//set函数
	//设置上一个文法
	static void setPreviousGrammar(Grammar& grammar) {
		previousGrammar = grammar;
	}


	//总的函数调用
	static void extractLeftCommonFactor() {
		//推导产生式
		deduceAllProductions();

		//对每个非终结符进行提取左公因子
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();//获得所有符号
		for (auto& symbol : allSymbol) {
			//如果是终结符，不用处理
			if (symbol.isNonTerminator() == false) {
				continue;
			}

			//非终结符
			TreeNode* treeRoot = LeftCommonFactorTree::createTreeRoot();//创建树的根节点
			LeftCommonFactorTree processingTree(treeRoot, symbol);//创建树的对象
			vector<Production> rootProductions = deducedProductionsMap[symbol];//根节点对应的符号的代入后的所有产生式
			for (auto& rootProduction : rootProductions) {
				//逐一添加产生式到树中
				processingTree.addProduction(rootProduction);
			}
			//提取公因子，产生式在函数中插入到map集合中
			processingTree.extractLeftCommonFactor(treeRoot, grammar.getProductionsMap());

			//释放树节点的内存
			processingTree.releaseMemory();
		}

		//输出
		printAll();
	}
};


map<Symbol, vector<Production>> LeftCommonFactorExtractor::deducedProductionsMap;//保存代入推导后得到的所有产生式
Grammar LeftCommonFactorExtractor::grammar;//提取左公因子后的文法
Grammar& LeftCommonFactorExtractor::previousGrammar = LeftRecursionEliminator::getGrammar();//上一个功能进行后的文法
