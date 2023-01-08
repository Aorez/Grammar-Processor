#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Util.h"


using namespace std;


//字母表的静态类，存储所有用到的终结符和非终结符
static class Alphabet {
private:
	static map<string, Symbol> symbolMap;//字符串到符号类的映射集合

	static string epsilonLabel;//空字的字符串表示，“@”

	static map<Symbol, int> copyCountMap;//记录A'、A''等的数量，提取公因子、消除左递归中需要新建符号，因此需要记录数量，避免符号重复

public:
	//get函数
	//throw()可以不用写
	static Symbol getSymbol(string label) throw() {
		//调用静态工具类的map查找函数
		return Util::getMapValue(symbolMap, label);
	}
	static Symbol getSymbol(char label) throw() {
		return getSymbol(string(1, label));
	}
	//返回所有符号的一个vector
	static vector<Symbol> getAllSymbol() {
		return Util::getMapValueAsVector(symbolMap);
	}
	//返回所有终结符
	static vector<Symbol> getAllTerminator() {
		vector<Symbol> allTerminator;
		for (auto symbol : getAllSymbol()) {
			if (symbol.isNonTerminator() == false) {
				allTerminator.push_back(symbol);
			}
		}
		return allTerminator;
	}
	//返回空字的符号类对象
	static Symbol getEpsilonSymbol() {
		return addSymbol(epsilonLabel, Symbol::TERMINATOR);
	}


	//向字母表中增加符号
	static Symbol addSymbol(string label, bool isTerminator) {
		Util::addInitialValueToMap(symbolMap, label, Symbol(label, isTerminator));
		return getSymbol(label);
	}
	//向字母表中增加符号
	static Symbol addSymbol(char label, bool isTerminator) {
		return addSymbol(string(1, label), isTerminator);
	}
	//增加一个符号的拷贝，A'，A''
	static Symbol addCopy(Symbol symbolToCopy) {
		//如果map集合中还没有该符号，初始化拷贝次数为0
		if (copyCountMap.find(symbolToCopy) == copyCountMap.end()) {
			copyCountMap[symbolToCopy] = 0;
		}

		//拷贝次数加1
		int copyCount = ++copyCountMap[symbolToCopy];
		string copyLabel = symbolToCopy.getLabel();
		for (int i = 1; i <= copyCount; i++) {
			copyLabel.push_back('\'');
		}

		//创建符号
		return addSymbol(copyLabel, Symbol::NONTERMINATOR);
	}


	//将字母表中的某个符号删除
	static void removeSymbol(string label) {
		symbolMap.erase(label);
	}


	//输出字母表
	static void printAllSymbol() {
		vector<string> terminators;//终结符集合
		vector<string> nonterminators;//非终结符集合
		for (auto it : symbolMap) {
			//如果是终结符，放入终结符集合
			if (it.second.isNonTerminator() == false) {
				terminators.push_back(it.second.getLabel());
			}
			else {
				//放入非终结符集合
				nonterminators.push_back(it.second.getLabel());
			}
		}
		sort(terminators.begin(), terminators.end());
		sort(nonterminators.begin(), nonterminators.end());
		Util::printVectorInALine(terminators, string(" "), string("terminators"));
		Util::printVectorInALine(nonterminators, string(" "), string("nonterminators"));
	}


	//判断符号是否空字
	static bool isEpsilon(Symbol symbol) {
		return symbol.getLabel() == epsilonLabel;
	}
	static bool isEpsilon(string label) {
		return label == epsilonLabel;
	}


	//清空缓存
	static void clear() {
		copyCountMap.clear();
		symbolMap.clear();
	}
};


