#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"BaseSymbolSet.h"


using namespace std;


//文法符号串的first集合的类，即多个符号的first集合，类似产生式
class SymbolsFirstSet :public BaseSymbolSet {
private:
	vector<Symbol> keys;//文法符号串

public:
	//构造函数
	SymbolsFirstSet(vector<Symbol> keys) {
		this->keys = keys;
	}
	SymbolsFirstSet(Symbol symbol, set<Symbol> symbolSet, bool hasEpsilon) :BaseSymbolSet(symbol, symbolSet, hasEpsilon) {
	}


	//get函数
	set<Symbol> getFirstSet() {
		return getSymbolSet();
	}
};
