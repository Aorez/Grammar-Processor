#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Util.h"


using namespace std;


//��ĸ��ľ�̬�࣬�洢�����õ����ս���ͷ��ս��
static class Alphabet {
private:
	static map<string, Symbol> symbolMap;//�ַ������������ӳ�伯��

	static string epsilonLabel;//���ֵ��ַ�����ʾ����@��

	static map<Symbol, int> copyCountMap;//��¼A'��A''�ȵ���������ȡ�����ӡ�������ݹ�����Ҫ�½����ţ������Ҫ��¼��������������ظ�

public:
	//get����
	//throw()���Բ���д
	static Symbol getSymbol(string label) throw() {
		//���þ�̬�������map���Һ���
		return Util::getMapValue(symbolMap, label);
	}
	static Symbol getSymbol(char label) throw() {
		return getSymbol(string(1, label));
	}
	//�������з��ŵ�һ��vector
	static vector<Symbol> getAllSymbol() {
		return Util::getMapValueAsVector(symbolMap);
	}
	//���������ս��
	static vector<Symbol> getAllTerminator() {
		vector<Symbol> allTerminator;
		for (auto symbol : getAllSymbol()) {
			if (symbol.isNonTerminator() == false) {
				allTerminator.push_back(symbol);
			}
		}
		return allTerminator;
	}
	//���ؿ��ֵķ��������
	static Symbol getEpsilonSymbol() {
		return addSymbol(epsilonLabel, Symbol::TERMINATOR);
	}


	//����ĸ�������ӷ���
	static Symbol addSymbol(string label, bool isTerminator) {
		Util::addInitialValueToMap(symbolMap, label, Symbol(label, isTerminator));
		return getSymbol(label);
	}
	//����ĸ�������ӷ���
	static Symbol addSymbol(char label, bool isTerminator) {
		return addSymbol(string(1, label), isTerminator);
	}
	//����һ�����ŵĿ�����A'��A''
	static Symbol addCopy(Symbol symbolToCopy) {
		//���map�����л�û�и÷��ţ���ʼ����������Ϊ0
		if (copyCountMap.find(symbolToCopy) == copyCountMap.end()) {
			copyCountMap[symbolToCopy] = 0;
		}

		//����������1
		int copyCount = ++copyCountMap[symbolToCopy];
		string copyLabel = symbolToCopy.getLabel();
		for (int i = 1; i <= copyCount; i++) {
			copyLabel.push_back('\'');
		}

		//��������
		return addSymbol(copyLabel, Symbol::NONTERMINATOR);
	}


	//����ĸ���е�ĳ������ɾ��
	static void removeSymbol(string label) {
		symbolMap.erase(label);
	}


	//�����ĸ��
	static void printAllSymbol() {
		vector<string> terminators;//�ս������
		vector<string> nonterminators;//���ս������
		for (auto it : symbolMap) {
			//������ս���������ս������
			if (it.second.isNonTerminator() == false) {
				terminators.push_back(it.second.getLabel());
			}
			else {
				//������ս������
				nonterminators.push_back(it.second.getLabel());
			}
		}
		sort(terminators.begin(), terminators.end());
		sort(nonterminators.begin(), nonterminators.end());
		Util::printVectorInALine(terminators, string(" "), string("terminators"));
		Util::printVectorInALine(nonterminators, string(" "), string("nonterminators"));
	}


	//�жϷ����Ƿ����
	static bool isEpsilon(Symbol symbol) {
		return symbol.getLabel() == epsilonLabel;
	}
	static bool isEpsilon(string label) {
		return label == epsilonLabel;
	}


	//��ջ���
	static void clear() {
		copyCountMap.clear();
		symbolMap.clear();
	}
};


