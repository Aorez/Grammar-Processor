#pragma once
#include<bits/stdc++.h>


#include"BaseSymbolSet.h"


using namespace std;


//first���ϵ���
class FirstSet :public BaseSymbolSet {
private:

public:
	//���캯��
	//�޲ι��캯��
	FirstSet() {
	}
	//����Ϊ���ϵķ���
	FirstSet(Symbol symbolKey) :BaseSymbolSet(symbolKey) {

	}


	//get����
	//���first����
	set<Symbol> getFirstSet() {
		return getSymbolSet();
	}
};
