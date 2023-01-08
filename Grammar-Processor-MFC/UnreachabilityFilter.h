#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Grammar.h"


#include"Util.h"
#include"Alphabet.h"
#include"TerminationFilter.h"


using namespace std;


static class UnreachabilityFilter {
private:
	static map<Symbol, int> recordMap;//��¼�����Ƿ�ɵ���

	static vector<vector<Production>> unreachableProductions;//���治�ɵ���Ĳ���ʽ

	static Grammar grammar;//ɾ�����ɵ������ķ�

	static Grammar& previousGrammar;//��һ�����ܽ��к���ķ�

	static Grammar& previousGrammar2;//��һ���ķ�

	static int previous;//��һ���ķ��ı�־


	//�Լ�¼�����Ƿ�ɵ����map���Ͻ��г�ʼ��
	//�ķ���ʼ���ż�¼Ϊ�ɵ���
	static void initializeRecordMap() {
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();//ȫ�����ŵ�vector
		for (auto& it : allSymbol) {
			if (it.isNonTerminator()) {
				Util::addInitialValueToMap(recordMap, it, int(reachabilityRecord::UNCHECKED));
			}
		}
		recordMap[Grammar::getStart()] = reachabilityRecord::REACHABLE;
	}


	//�ӿ�ʼ���ŵķ��ս����ʼ���й�����������������������ľ��ǿɵ����
	static void checkReachability() {
		Grammar* pp = nullptr;
		if (previous == 1) {
			pp = &previousGrammar;
		}
		else {
			pp = &previousGrammar2;
		}
		Symbol start = Grammar::getStart();//����ķ���ʼ����
		stack<Symbol> uncheckedStack;//�ɴ���δ���ķ��ŵ�ջ�����ڹ����������
		uncheckedStack.push(start);//BFS�Ŀ�ʼ

		//�ӿ�ʼ���ſ�ʼ�����й�������������ս�����������ͱ��Ϊ�ɴﵽ
		Symbol symbol;//ѭ����ʹ��
		//ջΪ�����˳�ѭ��
		while (!uncheckedStack.empty()) {
			symbol = uncheckedStack.top();
			uncheckedStack.pop();

			vector<Production> productions = pp->getProductions(symbol);//������в���ʽ
			//������ǰ���ŵ����в���ʽ
			for (auto& p : productions) {
				//������ǰ����ʽ�Ҳ������з���
				vector<Symbol> ss = p.getRight();
				for (auto& s : ss) {
					//����÷����Ƿ��ս��
					//��Ҫ�ж����Ŀɴ��Լ�¼
					if (s.isNonTerminator()) {
						//����ɴ���δ���
						if (recordMap[s] == reachabilityRecord::UNCHECKED) {
							//������Ϊ�ɴ�
							recordMap[s] = reachabilityRecord::REACHABLE;
							//����ջ�У����к���BFS
							uncheckedStack.push(s);
						}
					}
				}
			}
		}
	}


	//�Կɴ��Խ��м��֮�󣬾�Ҫ�Բ��ɵ���Ĳ���ʽ����ɾ��
	//���м��֮�󣬿ɵ���Ķ�������Ϊ��REACHABLE�������ɴ�Ļ����ʼ��UNCHECKED
	static void deleteUnreachabilities() {
		Grammar* pp = nullptr;
		if (previous == 1) {
			pp = &previousGrammar;
		}
		else {
			pp = &previousGrammar2;
		}

		map<Symbol, vector<Production>>& productionsMap = pp->getProductionsMap();//��ȡ���в���ʽ�ļ���
		for (auto& it : productionsMap) {
		//for (map<Symbol, vector<Production>>::iterator it = productionsMap.begin(); it != productionsMap.end();) {
			Symbol left = it.first;//��ǰ����

			//����ǿɵ����
			if (recordMap[left] == reachabilityRecord::REACHABLE) {
				//��ӵ����ķ���
				grammar.addProductions(left, it.second);
			}
			//����������ǲ��ɴﵽ�ģ������
			else {
				unreachableProductions.push_back(it.second);
				//����ĸ���еĲ��ɵ���ķ���ɾ��
				//erase֮��leftָ��ı䣬������ɾ������
				Alphabet::removeSymbol(left.getLabel());
			}
		}
	}


	//��ջ���
	static void clearCache() {
		recordMap.clear();
		unreachableProductions.clear();
		grammar = Grammar();
	}

public:
	static enum reachabilityRecord { REACHABLE, UNREACHABLE, UNCHECKED };//�ڼ�¼�����Ƿ�ɵ����map�����У��ֱ��ʾ�ɵ�����ɵ��δ���

	//get����
	//����ķ�
	static Grammar& getGrammar() {
		return grammar;
	}
	//���ص���vector<string>
	static vector<string> getUnreachableProductions() {
		vector<string> result;
		for (auto v : unreachableProductions) {
			result.push_back(Util::vectorToString(v, "\n"));
		}
		return result;
	}


	//set����
	//������һ�ε��ķ�
	static void setPreviousGrammar(Grammar& grammar) {
		previousGrammar = grammar;
	}
	//������һ���ķ��ı�־
	static void setPrevious(int pre) {
		previous = pre;
	}


	//�ܵĵ��ú���
	static void filterUnreachability() {
		clearCache();

		//��ʼ����¼�Ƿ�ɴ��map����
		initializeRecordMap();

		//��ʼ���Ų�����
		Symbol start = Grammar::getStart();//����ķ���ʼ����
		if (previousGrammar.existLeft(start) == false) {
		}
		else {
			//��鲻�ɴ���
			checkReachability();
		}

		//ɾ�����ɴ�
		deleteUnreachabilities();

		//���
		cout << "\nunreachableProductions\n";
		Util::printVectorInLines(unreachableProductions);
		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}
};


