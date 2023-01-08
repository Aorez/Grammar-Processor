#pragma once
#include<bits/stdc++.h>


#include"GrammarPretreatment.h"
#include"TerminationFilter.h"
#include"UnreachabilityFilter.h"
#include"LeftCommonFactorExtractor.h"
#include"LeftRecursionEliminator.h"
#include"StateGraph.h"


using namespace std;


//总的功能调用的静态类
static class GrammarProcessor {
private:

public:
	static void processGrammar() {
		//文法预处理
		GrammarPretreatment::pretreatGrammar();

		//删除不可终止的产生式
		TerminationFilter::filterTermination();

		//删除不可到达的产生式
		UnreachabilityFilter::filterUnreachability();

		//输出字母表
		Alphabet::printAllSymbol();

		//左递归
		LeftRecursionEliminator::eliminateLeftRecursion();

		//提取左公因子
		LeftCommonFactorExtractor::extractLeftCommonFactor();

		//删除不可到达
		UnreachabilityFilter::filterUnreachability();

		//first集合follow集合
		Grammar& grammar = LeftCommonFactorExtractor::getGrammar();
		cout << "\nfirstSetMap\n";
		grammar.printFirstSetMap();
		cout << "\nfollowSetMap\n";
		grammar.printFollowSetMap();

		if (grammar.isLeftLinear()) {
			cout << "左线性\n";
		}
		if (grammar.isRightLinear()) {
			cout << "右线性\n";
		}
		StateGraph sg(grammar);
	}


};
