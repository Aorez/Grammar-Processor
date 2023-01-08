#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Production.h"
#include"Grammar.h"
#include"LeftCommonFactorTree.h"
#include"Util.h"
#include"LeftRecursionEliminator.h"


using namespace std;


//��ȡ�����ӵľ�̬��
static class LeftCommonFactorExtractor {
private:
	const static int DEFAULT_SUBSTITUTION_TIMES = 3;//Ĭ�ϵĴ�����ս������ʽ�����Ƶ��Ĵ������

	static map<Symbol, vector<Production>> deducedProductionsMap;//��������Ƶ���õ������в���ʽ

	static Grammar grammar;//��ȡ�����Ӻ���ķ�

	static Grammar& previousGrammar;//��һ�����ܽ��к���ķ�


	//��ĳһ������ʽ��ĳһ���Ҳ����з��ս���Ĵ��룬����1��
	//��Ϊ��������ʽ�п��ܴ�һ����ɶ�������Է��ؼ���
	//���صļ����п������ظ��Ĳ���ʽ
	//����A->BCD|CB��B->Ef|Fg��C->fg��D->hH|iI
	//����A->BCD���¼��ϣ�A->EfCD��A->FgCD
	//A->EffgD��A->FgfgD
	//����A->BCD�����¼����е�ÿһ����C��fg����D��hH|iI
	//D�����hH��iI����ÿһ��ñ�������2����˼
	//���Բ�ȫ�����룬ֻ����A->BaC��B
	static vector<Production> substituteProduction(Production productionToSubstitute) {
		//��ǰ��������Ӧ��������A->BCD������

		Symbol left = productionToSubstitute.getLeft();//��
		vector<Symbol> originalRight = productionToSubstitute.getRight();//��¼ԭ�����Ҳ���BCD
		vector<Production> newProductions;//���һ������ʽ�п��ܱ�ɶ��������Ϊ����

		//A->B�������ռ��ϱ��A->Ef|Fg
		//����������󲿷���һ���������д���
			
		//bool terminatorAppear = false;//�Ƿ�������ս�����ռ�������ķ��ռ�����������
		for (auto originalSymbol : originalRight) {
			//��������ǿ������ţ����ô���
			if (originalSymbol.isCopySymbol()
				|| originalSymbol.isNonTerminator() == false
				|| originalSymbol == left
				/*|| terminatorAppear*/) {
				//terminatorAppear = true;

				//����ǿյģ�Ҫ�½�
				if (newProductions.empty()) {
					newProductions.push_back(Production(left, vector<Symbol>({ originalSymbol })));
					continue;
				}

				//��ÿ�����Ҳ��������Ӹ��ս��
				//������ս��������һ������ʽ��������
				for (auto& newProduction : newProductions) {
					newProduction.addRightSymbol(originalSymbol);
				}
			}
			else {
				//D->hH|iI
				vector<Production> originalProductions = previousGrammar.getProductions(originalSymbol);//��÷��ŵ����в���ʽ��׼������
				for (vector<Production>::iterator it = originalProductions.begin(); it != originalProductions.end();) {
					//�ղ���ʽ���ô���
					if (it->isEpsilon()) {
						it = originalProductions.erase(it);
						continue;
					}

					//���S->A��A->aA'���򲻴���A
					if (it->hasCopySymbol()) {
						it = originalProductions.erase(it);
						continue;
					}

					it++;
				}
				//����ǰ����޳���originalProductions�п���Ϊ��
				//����Ҫ��������Ĵ��뵽����
				//����������ս��
				if (originalProductions.empty()) {
					//����ǿյģ�Ҫ�½�
					if (newProductions.empty()) {
						newProductions.push_back(Production(left, vector<Symbol>({ originalSymbol })));
						continue;
					}

					//��ÿ�����Ҳ��������Ӹ��ս��
					//������ս��������һ������ʽ��������
					for (auto& newProduction : newProductions) {
						newProduction.addRightSymbol(originalSymbol);
					}
					continue;

				}

				if (newProductions.empty()) {
					//�ڲ���֮ǰҪ�޸���
					for (auto& firstProduction : originalProductions) {
						firstProduction.setLeft(left);
					}
					newProductions.insert(newProductions.end(), originalProductions.begin(), originalProductions.end());
					//���ʱ��newRights��������ж������ʽ��
					continue;
				}

				//A->EffgD��A->FgfgD
				vector<Production> previousNewRights(newProductions);//�������Ҳ������ݣ�׼���滻
				//���֮������滻
				newProductions.clear();
				//��ÿһ���Ҳ�֮�У�A->EffgD
				for (auto& previousNewRight : previousNewRights) {
					//��ÿһ������ʽ֮�У�D->hH
					for (auto& originalProduction : originalProductions) {
						newProductions.push_back(Production(left, { previousNewRight.getRight(),originalProduction.getRight() }));
					}
				}
			}
		}
		return newProductions;
	}


	//ͨ��������ս�����󲿵����в���ʽ�����Ƶ�
	//ָ���������ȣ�������Ĵ�����Ĭ����3��
	//S->aA��A->B��B->b�����S->aB���Ǵ���1��
	//����ȡ���������õ�
	static vector<Production> deduceProductions(Symbol left, int substitutionTimes = DEFAULT_SUBSTITUTION_TIMES) {
		vector<Production> productions = previousGrammar.getProductions(left);//������в���ʽ
		vector<Production> substitutions(productions);//�����Ĳ���ʽ

		int times = 0;
		while (true) {
			//��������ﵽҪ���˳�ѭ��
			if (times == substitutionTimes) {
				break;
			}
			times++;

			vector<Production> previousSubstitutions(substitutions);//��һ�δ������ʽ�Ľ��
			//���֮������滻
			substitutions.clear();
			//���������Ҳ�
			//��ÿһ���Ҳ����д���
			for (Production& previousSubstitutioin : previousSubstitutions) {
				vector<Production> substitution = substituteProduction(previousSubstitutioin);
				//���뵽���Ľ������
				substitutions.insert(substitutions.end(), substitution.begin(), substitution.end());
			}
		}

		return substitutions;
	}


	//�������в���ʽ�Ĵ����Ƶ���Ȼ�󱣴浽������
	static void deduceAllProductions() {
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();
		for (auto& symbol : allSymbol) {
			//������ս�������ô���
			if (symbol.isNonTerminator() == false) {
				continue;
			}
			deducedProductionsMap[symbol] = deduceProductions(symbol);
		}
	}


	//���
	static void printAll() {
		cout << "\ndeducedProductionsMap\n";
		Util::printMapValueInLines(deducedProductionsMap);

		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}

public:
	//get����
	static map<Symbol, vector<Production>>& getDeducedProductionsMap() {
		return deducedProductionsMap;
	}
	static Grammar& getGrammar() {
		return grammar;
	}


	//set����
	//������һ���ķ�
	static void setPreviousGrammar(Grammar& grammar) {
		previousGrammar = grammar;
	}


	//�ܵĺ�������
	static void extractLeftCommonFactor() {
		//�Ƶ�����ʽ
		deduceAllProductions();

		//��ÿ�����ս��������ȡ������
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();//������з���
		for (auto& symbol : allSymbol) {
			//������ս�������ô���
			if (symbol.isNonTerminator() == false) {
				continue;
			}

			//���ս��
			TreeNode* treeRoot = LeftCommonFactorTree::createTreeRoot();//�������ĸ��ڵ�
			LeftCommonFactorTree processingTree(treeRoot, symbol);//�������Ķ���
			vector<Production> rootProductions = deducedProductionsMap[symbol];//���ڵ��Ӧ�ķ��ŵĴ��������в���ʽ
			for (auto& rootProduction : rootProductions) {
				//��һ��Ӳ���ʽ������
				processingTree.addProduction(rootProduction);
			}
			//��ȡ�����ӣ�����ʽ�ں����в��뵽map������
			processingTree.extractLeftCommonFactor(treeRoot, grammar.getProductionsMap());

			//�ͷ����ڵ���ڴ�
			processingTree.releaseMemory();
		}

		//���
		printAll();
	}
};


map<Symbol, vector<Production>> LeftCommonFactorExtractor::deducedProductionsMap;//��������Ƶ���õ������в���ʽ
Grammar LeftCommonFactorExtractor::grammar;//��ȡ�����Ӻ���ķ�
Grammar& LeftCommonFactorExtractor::previousGrammar = LeftRecursionEliminator::getGrammar();//��һ�����ܽ��к���ķ�
