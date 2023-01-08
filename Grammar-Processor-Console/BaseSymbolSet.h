#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Alphabet.h"


using namespace std;


//���ż��ϵĻ���
class BaseSymbolSet {
protected:
	Symbol symbolKey;//�����ĸ����ŵķ��ż���

	bool hasepsilon;//�Ƿ��п��֣���¼�����Ƿ����Ƴ�����

	set<Symbol> symbolSet;//���ż���

public:
	//���캯��
	//Ĭ�Ϲ��캯��
	//ί�й��캯��
	BaseSymbolSet() :BaseSymbolSet(Symbol()) {
	}
	//����key�������ĸ����ŵ�first����
	BaseSymbolSet(Symbol key) :BaseSymbolSet(key, set<Symbol>()) {
	}
	//��first���Ϻ�ĳ�����Ž��й���
	BaseSymbolSet(Symbol key, set<Symbol> symbolSet) :BaseSymbolSet(key, symbolSet, false) {
	}
	//���캯���Ĵ���
	BaseSymbolSet(Symbol key, set<Symbol> symbolSet, bool hasEpsilon) {
		this->symbolKey = key;
		this->symbolSet = symbolSet;
		this->hasepsilon = hasEpsilon;
	}



	//set����
	void setHasEpsilon(bool hasEpsilon) {
		this->hasepsilon = hasEpsilon;
	}


	//get����
	set<Symbol> getSymbolSet() {
		return this->symbolSet;
	}
	bool getHasEpsilon() {
		return this->hasepsilon;
	}
	Symbol getSymbolKey() {
		return this->symbolKey;
	}


	//���������
	//�������
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


	//��first�����в���
	void addSymbol(Symbol symbolToAdd) {
		symbolSet.insert(symbolToAdd);
	}
	//��first�����в�����һ��first�����е�ȫ������
	void addSymbolSet(BaseSymbolSet symbolSetToAdd) {
		for (auto symbol : symbolSetToAdd.getSymbolSet()) {
			addSymbol(symbol);
		}
	}


	//�жϷ��ż����Ƿ��п���
	bool hasEpsilon() {
		return this->hasepsilon;
	}
};
