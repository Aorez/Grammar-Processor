#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Production.h"


#include"Util.h"


using namespace std;


//消除左递归的类
static class LeftRecursionEliminator {
private:
	static Grammar grammar;//消除左递归后的文法

	static Grammar& previousGrammar;//上一个功能进行后的文法

	//顺序扫描所有左部，并记录扫描过的左部
	//遍历当前正在扫描的左部的右部，如果右部中有符号已经被扫描过，就出现了间接左递归
	//需要进行消除
	//消除后再进行当前符号的直接左递归的消除
	static void eliminateIndirectLeftRecursion() {
		//遍历每一个左部及其对应所有右部
		//产生式集合来自提取公因子的类
		for (auto& leftAndRights : previousGrammar.getProductionsMap()) {
			vector<Production> productionsEliminated;//该左部消除了间接左递归之后的产生式
			//遍历每一个产生式
			for (auto& production : leftAndRights.second) {
				//[]是重载运算符
				Symbol firstSymbol = production[0];//获得该产生式右部的第一个符号

				//如果该符号还没有被扫描过，产生式不用改变
				if (grammar.getProductionsMap().find(firstSymbol) == grammar.getProductionsMap().end()) {
					productionsEliminated.push_back(production);
				}
				else {
					vector<Production>& firstProductions = grammar.getProductionsMap()[firstSymbol];//被扫描过的符号的所有产生式
					//S->Aa|b，A->Ac|Sd，假设S先被扫描过了
					//[Aa, b]替换产生式[Sd]，变成[Aad, bd]
					//遍历每一个要替换的产生式，对该右部的第一个符号进行替换
					for (auto& firstProduction : firstProductions) {
						//原来的左部->第一个符号的产生式 加上 原来的右部截取后的内容
						productionsEliminated.push_back(Production(leftAndRights.first, { firstProduction.getRight(),production.cutRight(1) }));
					}
				}
			}

			//消除直接左递归
			eliminateDirectLeftRecursion(leftAndRights.first, productionsEliminated);
		}
	}


	//消除直接左递归
	//P->Pa | Pb | Pc | Pd | e | f | g | h
	//转成
	//P->eP'|fP' | gP'|hP'
	//P'-> aP' | bP'|cP' | dP'|ε
	//消除一个符号的直接左递归
	//参数为消除了间接左递归之后的产生式
	static void eliminateDirectLeftRecursion(Symbol left, vector<Production> productionsEliminated) {
		vector<Production> leftRecursionProductions;//含有左递归的右部产生式集合
		vector<Production> noLeftRecursionProductions;//不含有左递归的右部产生式集合

		//添加到集合中
		for (auto& production : productionsEliminated) {
			//调用函数判断是否左递归
			//然后添加到对应集合中
			if (production.isLeftRecursion()) {
				leftRecursionProductions.push_back(production);
			}
			else {
				noLeftRecursionProductions.push_back(production);
			}
		}

		//如果不含有左递归
		if (leftRecursionProductions.empty()) {
			//直接插入结果
			grammar.getProductionsMap()[left] = productionsEliminated;
			return;
		}

		//P->Pa | Pb | Pc | Pd | e | f | g | h
		//转成
		//P->eP'|fP' | gP'|hP'
		//P'-> aP' | bP'|cP' | dP'|ε
		Symbol copyLeft = Alphabet::addCopy(left);//创建新符号
		vector<Production> copyProductions;//保存左递归消除的结果，新符号
		vector<Production> originalProductions;//保存左递归消除的结果，原来的符号

		//加P'到没有左递归的产生式的末尾
		for (auto& production : noLeftRecursionProductions) {
			vector<Symbol> originalRight;
			//如果是空产生式，A->Aa|@，A->A'，A'->aA'|@，其中，A->A'，@不用加进去
			if (production.isEpsilon() == false) {
				originalRight = production.getRight();
			}
			//原来的左部->没有左递归的产生式 加上 新的符号
			//插入到结果集
			originalProductions.push_back(Production(left, { originalRight,{copyLeft} }));
		}

		//新建P'的产生式
		for (auto& production : leftRecursionProductions) {
			//新的左部->有左递归的产生式的截取 加上 新符号
			copyProductions.push_back(Production(copyLeft, { production.cutRight(1),{copyLeft} }));
		}
		//插入P'->空字
		copyProductions.push_back(Production(copyLeft, { Alphabet::getEpsilonSymbol() }));

		//放入总的集合
		grammar.getProductionsMap()[left] = originalProductions;
		grammar.getProductionsMap()[copyLeft] = copyProductions;
	}

public:
	//get函数
	//获得文法
	static Grammar& getGrammar() {
		return grammar;
	}


	//set函数
	//设置上一个文法
	static void setPreviousGrammar(Grammar& grammar) {
		previousGrammar = grammar;
	}


	//总的函数调用
	static void eliminateLeftRecursion() {
		//消除间接左递归
		eliminateIndirectLeftRecursion();

		//输出
		cout << "\nproductionsMap\n";
		Util::printMapValueInLines(grammar.getProductionsMap());
	}
};


Grammar& LeftRecursionEliminator::previousGrammar = UnreachabilityFilter::getGrammar();//上一个功能进行后的文法
Grammar LeftRecursionEliminator::grammar;//消除左递归后的文法
