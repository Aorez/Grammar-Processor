#pragma once
#include<bits/stdc++.h>


#include"BaseSymbolSet.h"


using namespace std;


//follow集合的类
class FollowSet :public BaseSymbolSet {
private:

public:
	//构造函数
	//无参构造函数
	FollowSet() {
	}
	//参数为集合的符号
	FollowSet(Symbol symbolKey) :BaseSymbolSet(symbolKey) {
	}


	//get函数
	//获得follow集合
	set<Symbol> getFollowSet(Symbol symbolKey) {
		return getSymbolSet();
	}
};
