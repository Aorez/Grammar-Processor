#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"BaseSymbolSet.h"


using namespace std;


//�ķ����Ŵ���first���ϵ��࣬��������ŵ�first���ϣ����Ʋ���ʽ
class SymbolsFirstSet :public BaseSymbolSet {
private:
	vector<Symbol> keys;//�ķ����Ŵ�

public:
	//���캯��
	SymbolsFirstSet(vector<Symbol> keys) {
		this->keys = keys;
	}
	SymbolsFirstSet(Symbol symbol, set<Symbol> symbolSet, bool hasEpsilon) :BaseSymbolSet(symbol, symbolSet, hasEpsilon) {
	}


	//get����
	set<Symbol> getFirstSet() {
		return getSymbolSet();
	}
};
