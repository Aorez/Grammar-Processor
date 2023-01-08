#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Production.h"


#include"Util.h"


using namespace std;


//������ݹ����
static class LeftRecursionEliminator {
private:
	static Grammar grammar;//������ݹ����ķ�

	static Grammar& previousGrammar;//��һ�����ܽ��к���ķ�

	//˳��ɨ�������󲿣�����¼ɨ�������
	//������ǰ����ɨ����󲿵��Ҳ�������Ҳ����з����Ѿ���ɨ������ͳ����˼����ݹ�
	//��Ҫ��������
	//�������ٽ��е�ǰ���ŵ�ֱ����ݹ������
	static void eliminateIndirectLeftRecursion() {
		//����ÿһ���󲿼����Ӧ�����Ҳ�
		//����ʽ����������ȡ�����ӵ���
		for (auto& leftAndRights : previousGrammar.getProductionsMap()) {
			vector<Production> productionsEliminated;//���������˼����ݹ�֮��Ĳ���ʽ
			//����ÿһ������ʽ
			for (auto& production : leftAndRights.second) {
				//[]�����������
				Symbol firstSymbol = production[0];//��øò���ʽ�Ҳ��ĵ�һ������

				//����÷��Ż�û�б�ɨ���������ʽ���øı�
				if (grammar.getProductionsMap().find(firstSymbol) == grammar.getProductionsMap().end()) {
					productionsEliminated.push_back(production);
				}
				else {
					vector<Production>& firstProductions = grammar.getProductionsMap()[firstSymbol];//��ɨ����ķ��ŵ����в���ʽ
					//S->Aa|b��A->Ac|Sd������S�ȱ�ɨ�����
					//[Aa, b]�滻����ʽ[Sd]�����[Aad, bd]
					//����ÿһ��Ҫ�滻�Ĳ���ʽ���Ը��Ҳ��ĵ�һ�����Ž����滻
					for (auto& firstProduction : firstProductions) {
						//ԭ������->��һ�����ŵĲ���ʽ ���� ԭ�����Ҳ���ȡ�������
						productionsEliminated.push_back(Production(leftAndRights.first, { firstProduction.getRight(),production.cutRight(1) }));
					}
				}
			}

			//����ֱ����ݹ�
			eliminateDirectLeftRecursion(leftAndRights.first, productionsEliminated);
		}
	}


	//����ֱ����ݹ�
	//P->Pa | Pb | Pc | Pd | e | f | g | h
	//ת��
	//P->eP'|fP' | gP'|hP'
	//P'-> aP' | bP'|cP' | dP'|��
	//����һ�����ŵ�ֱ����ݹ�
	//����Ϊ�����˼����ݹ�֮��Ĳ���ʽ
	static void eliminateDirectLeftRecursion(Symbol left, vector<Production> productionsEliminated) {
		vector<Production> leftRecursionProductions;//������ݹ���Ҳ�����ʽ����
		vector<Production> noLeftRecursionProductions;//��������ݹ���Ҳ�����ʽ����

		//��ӵ�������
		for (auto& production : productionsEliminated) {
			//���ú����ж��Ƿ���ݹ�
			//Ȼ����ӵ���Ӧ������
			if (production.isLeftRecursion()) {
				leftRecursionProductions.push_back(production);
			}
			else {
				noLeftRecursionProductions.push_back(production);
			}
		}

		//�����������ݹ�
		if (leftRecursionProductions.empty()) {
			//ֱ�Ӳ�����
			grammar.getProductionsMap()[left] = productionsEliminated;
			return;
		}

		//P->Pa | Pb | Pc | Pd | e | f | g | h
		//ת��
		//P->eP'|fP' | gP'|hP'
		//P'-> aP' | bP'|cP' | dP'|��
		Symbol copyLeft = Alphabet::addCopy(left);//�����·���
		vector<Production> copyProductions;//������ݹ������Ľ�����·���
		vector<Production> originalProductions;//������ݹ������Ľ����ԭ���ķ���

		//��P'��û����ݹ�Ĳ���ʽ��ĩβ
		for (auto& production : noLeftRecursionProductions) {
			vector<Symbol> originalRight;
			//����ǿղ���ʽ��A->Aa|@��A->A'��A'->aA'|@�����У�A->A'��@���üӽ�ȥ
			if (production.isEpsilon() == false) {
				originalRight = production.getRight();
			}
			//ԭ������->û����ݹ�Ĳ���ʽ ���� �µķ���
			//���뵽�����
			originalProductions.push_back(Production(left, { originalRight,{copyLeft} }));
		}

		//�½�P'�Ĳ���ʽ
		for (auto& production : leftRecursionProductions) {
			//�µ���->����ݹ�Ĳ���ʽ�Ľ�ȡ ���� �·���
			copyProductions.push_back(Production(copyLeft, { production.cutRight(1),{copyLeft} }));
		}
		//����P'->����
		copyProductions.push_back(Production(copyLeft, { Alphabet::getEpsilonSymbol() }));

		//�����ܵļ���
		grammar.getProductionsMap()[left] = originalProductions;
		grammar.getProductionsMap()[copyLeft] = copyProductions;
	}

public:
	//get����
	//����ķ�
	static Grammar& getGrammar() {
		return grammar;
	}


	//set����
	//������һ���ķ�
	static void setPreviousGrammar(Grammar& grammar) {
		previousGrammar = grammar;
	}


	//�ܵĺ�������
	static void eliminateLeftRecursion() {
		//���������ݹ�
		eliminateIndirectLeftRecursion();

		//���
		cout << "\nproductionsMap\n";
		Util::printMapValueInLines(grammar.getProductionsMap());
	}
};


Grammar& LeftRecursionEliminator::previousGrammar = UnreachabilityFilter::getGrammar();//��һ�����ܽ��к���ķ�
Grammar LeftRecursionEliminator::grammar;//������ݹ����ķ�
