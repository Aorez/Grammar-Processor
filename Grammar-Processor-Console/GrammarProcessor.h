#pragma once
#include<bits/stdc++.h>


#include"GrammarPretreatment.h"
#include"TerminationFilter.h"
#include"UnreachabilityFilter.h"
#include"LeftCommonFactorExtractor.h"
#include"LeftRecursionEliminator.h"
#include"StateGraph.h"


using namespace std;


//�ܵĹ��ܵ��õľ�̬��
static class GrammarProcessor {
private:

public:
	static void processGrammar() {
		//�ķ�Ԥ����
		GrammarPretreatment::pretreatGrammar();

		//ɾ��������ֹ�Ĳ���ʽ
		TerminationFilter::filterTermination();

		//ɾ�����ɵ���Ĳ���ʽ
		UnreachabilityFilter::filterUnreachability();

		//�����ĸ��
		Alphabet::printAllSymbol();

		//��ݹ�
		LeftRecursionEliminator::eliminateLeftRecursion();

		//��ȡ������
		LeftCommonFactorExtractor::extractLeftCommonFactor();

		//ɾ�����ɵ���
		UnreachabilityFilter::filterUnreachability();

		//first����follow����
		Grammar& grammar = LeftCommonFactorExtractor::getGrammar();
		cout << "\nfirstSetMap\n";
		grammar.printFirstSetMap();
		cout << "\nfollowSetMap\n";
		grammar.printFollowSetMap();

		if (grammar.isLeftLinear()) {
			cout << "������\n";
		}
		if (grammar.isRightLinear()) {
			cout << "������\n";
		}
		StateGraph sg(grammar);
	}


};
