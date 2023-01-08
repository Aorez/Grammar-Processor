#pragma once
#include<bits/stdc++.h>


#include"BaseSymbolSet.h"


using namespace std;


//first集合的类
class FirstSet :public BaseSymbolSet {
private:

public:
	//构造函数
	//无参构造函数
	FirstSet() {
	}
	//参数为集合的符号
	FirstSet(Symbol symbolKey) :BaseSymbolSet(symbolKey) {

	}


	//get函数
	//获得first集合
	set<Symbol> getFirstSet() {
		return getSymbolSet();
	}
};
