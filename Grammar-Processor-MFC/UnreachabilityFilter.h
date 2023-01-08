#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Grammar.h"


#include"Util.h"
#include"Alphabet.h"
#include"TerminationFilter.h"


using namespace std;


static class UnreachabilityFilter {
private:
	static map<Symbol, int> recordMap;//记录符号是否可到达

	static vector<vector<Production>> unreachableProductions;//保存不可到达的产生式

	static Grammar grammar;//删除不可到达后的文法

	static Grammar& previousGrammar;//上一个功能进行后的文法

	static Grammar& previousGrammar2;//上一个文法

	static int previous;//上一个文法的标志


	//对记录符号是否可到达的map集合进行初始化
	//文法开始符号记录为可到达
	static void initializeRecordMap() {
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();//全部符号的vector
		for (auto& it : allSymbol) {
			if (it.isNonTerminator()) {
				Util::addInitialValueToMap(recordMap, it, int(reachabilityRecord::UNCHECKED));
			}
		}
		recordMap[Grammar::getStart()] = reachabilityRecord::REACHABLE;
	}


	//从开始符号的非终结符开始进行广度优先搜索，可以搜索到的就是可到达的
	static void checkReachability() {
		Grammar* pp = nullptr;
		if (previous == 1) {
			pp = &previousGrammar;
		}
		else {
			pp = &previousGrammar2;
		}
		Symbol start = Grammar::getStart();//获得文法开始符号
		stack<Symbol> uncheckedStack;//可达性未检查的符号的栈，用于广度优先搜索
		uncheckedStack.push(start);//BFS的开始

		//从开始符号开始，进行广度优先搜索非终结符，搜索到就标记为可达到
		Symbol symbol;//循环中使用
		//栈为空则退出循环
		while (!uncheckedStack.empty()) {
			symbol = uncheckedStack.top();
			uncheckedStack.pop();

			vector<Production> productions = pp->getProductions(symbol);//获得所有产生式
			//遍历当前符号的所有产生式
			for (auto& p : productions) {
				//遍历当前产生式右部的所有符号
				vector<Symbol> ss = p.getRight();
				for (auto& s : ss) {
					//如果该符号是非终结符
					//则要判断它的可达性记录
					if (s.isNonTerminator()) {
						//如果可达性未检查
						if (recordMap[s] == reachabilityRecord::UNCHECKED) {
							//则设置为可达
							recordMap[s] = reachabilityRecord::REACHABLE;
							//推入栈中，进行后续BFS
							uncheckedStack.push(s);
						}
					}
				}
			}
		}
	}


	//对可达性进行检查之后，就要对不可到达的产生式进行删除
	//进行检查之后，可到达的都被设置为了REACHABLE，而不可达的还是最开始的UNCHECKED
	static void deleteUnreachabilities() {
		Grammar* pp = nullptr;
		if (previous == 1) {
			pp = &previousGrammar;
		}
		else {
			pp = &previousGrammar2;
		}

		map<Symbol, vector<Production>>& productionsMap = pp->getProductionsMap();//获取所有产生式的集合
		for (auto& it : productionsMap) {
		//for (map<Symbol, vector<Production>>::iterator it = productionsMap.begin(); it != productionsMap.end();) {
			Symbol left = it.first;//当前的左部

			//如果是可到达的
			if (recordMap[left] == reachabilityRecord::REACHABLE) {
				//添加到新文法中
				grammar.addProductions(left, it.second);
			}
			//其他情况都是不可达到的，不添加
			else {
				unreachableProductions.push_back(it.second);
				//将字母表中的不可到达的符号删除
				//erase之后left指向改变，所以先删除符号
				Alphabet::removeSymbol(left.getLabel());
			}
		}
	}


	//清空缓存
	static void clearCache() {
		recordMap.clear();
		unreachableProductions.clear();
		grammar = Grammar();
	}

public:
	static enum reachabilityRecord { REACHABLE, UNREACHABLE, UNCHECKED };//在记录符号是否可到达的map集合中，分别表示可到达，不可到达，未检查

	//get函数
	//获得文法
	static Grammar& getGrammar() {
		return grammar;
	}
	//返回的是vector<string>
	static vector<string> getUnreachableProductions() {
		vector<string> result;
		for (auto v : unreachableProductions) {
			result.push_back(Util::vectorToString(v, "\n"));
		}
		return result;
	}


	//set函数
	//设置上一次的文法
	static void setPreviousGrammar(Grammar& grammar) {
		previousGrammar = grammar;
	}
	//设置上一次文法的标志
	static void setPrevious(int pre) {
		previous = pre;
	}


	//总的调用函数
	static void filterUnreachability() {
		clearCache();

		//初始化记录是否可达的map集合
		initializeRecordMap();

		//开始符号不存在
		Symbol start = Grammar::getStart();//获得文法开始符号
		if (previousGrammar.existLeft(start) == false) {
		}
		else {
			//检查不可达性
			checkReachability();
		}

		//删除不可达
		deleteUnreachabilities();

		//输出
		cout << "\nunreachableProductions\n";
		Util::printVectorInLines(unreachableProductions);
		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}
};


