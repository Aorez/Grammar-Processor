#pragma once
#include<bits/stdc++.h>


#include"BaseSymbolSet.h"


using namespace std;


//follow���ϵ���
class FollowSet :public BaseSymbolSet {
private:

public:
	//���캯��
	//�޲ι��캯��
	FollowSet() {
	}
	//����Ϊ���ϵķ���
	FollowSet(Symbol symbolKey) :BaseSymbolSet(symbolKey) {
	}


	//get����
	//���follow����
	set<Symbol> getFollowSet(Symbol symbolKey) {
		return getSymbolSet();
	}
};
