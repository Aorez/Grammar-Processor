#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Alphabet.h"


using namespace std;


//符号集合的基类
class BaseSymbolSet {
protected:
	Symbol symbolKey;//属于哪个符号的符号集合

	bool hasepsilon;//是否有空字，记录集合是否能推出空字

	set<Symbol> symbolSet;//符号集合

public:
	//构造函数
	//默认构造函数
	//委托构造函数
	BaseSymbolSet() :BaseSymbolSet(Symbol()) {
	}
	//参数key，属于哪个符号的first集合
	BaseSymbolSet(Symbol key) :BaseSymbolSet(key, set<Symbol>()) {
	}
	//用first集合和某个符号进行构造
	BaseSymbolSet(Symbol key, set<Symbol> symbolSet) :BaseSymbolSet(key, symbolSet, false) {
	}
	//构造函数的传递
	BaseSymbolSet(Symbol key, set<Symbol> symbolSet, bool hasEpsilon) {
		this->symbolKey = key;
		this->symbolSet = symbolSet;
		this->hasepsilon = hasEpsilon;
	}



	//set函数
	void setHasEpsilon(bool hasEpsilon) {
		this->hasepsilon = hasEpsilon;
	}


	//get函数
	set<Symbol> getSymbolSet() {
		return this->symbolSet;
	}
	bool getHasEpsilon() {
		return this->hasepsilon;
	}
	Symbol getSymbolKey() {
		return this->symbolKey;
	}


	//运算符重载
	//重载输出
	friend ostream& operator<<(ostream& output, const BaseSymbolSet& symbolSetToPrint) {
		output << "{";
		for (auto symbol : symbolSetToPrint.symbolSet) {
			output << symbol << ", ";
		}
		if (symbolSetToPrint.hasepsilon) {
			output << Alphabet::getEpsilonSymbol() << ", ";
		}
		output << "}";
		return output;
	}


	//向first集合中插入
	void addSymbol(Symbol symbolToAdd) {
		symbolSet.insert(symbolToAdd);
	}
	//向first集合中插入另一个first集合中的全部符号
	void addSymbolSet(BaseSymbolSet symbolSetToAdd) {
		for (auto symbol : symbolSetToAdd.getSymbolSet()) {
			addSymbol(symbol);
		}
	}


	//判断符号集合是否有空字
	bool hasEpsilon() {
		return this->hasepsilon;
	}
};
