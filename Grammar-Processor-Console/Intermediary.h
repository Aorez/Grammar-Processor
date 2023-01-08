#pragma once
#include<bits/stdc++.h>


#include"Grammar.h"


#include"GrammarPretreatment.h"
#include"TerminationFilter.h"
#include"UnreachabilityFilter.h"
#include"LeftCommonFactorExtractor.h"
#include"LeftRecursionEliminator.h"


static class Intermediary {
public:
	//记录每一个类的名字，在获取每个类的grammar变量时使用
	static enum class ClassName {
		GrammarPretreatment,
		TerminationFilter,
		UnreachabilityFilter,
		LeftCommonFactorExtractor,
		LeftRecursionEliminator,
	};
	//获取某个类中的grammar变量
	//避免头文件的顺序包含，GrammarPretreatment、TerminationFilter等头文件就不用互相包含
	static Grammar& getGrammar(ClassName className) {
		switch (className) {
		case ClassName::GrammarPretreatment:return GrammarPretreatment::getGrammar();
		case ClassName::TerminationFilter:return TerminationFilter::getGrammar();
		case ClassName::UnreachabilityFilter:return UnreachabilityFilter::getGrammar();
		case ClassName::LeftCommonFactorExtractor:return LeftCommonFactorExtractor::getGrammar();
		case ClassName::LeftRecursionEliminator:return LeftRecursionEliminator::getGrammar();
		default:
			break;
		}
	}

};


enum class Intermediary::ClassName;//记录每一个类的名字，在获取每个类的grammar变量时使用
