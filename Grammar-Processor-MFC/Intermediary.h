#pragma once
#include<bits/stdc++.h>


#include"Grammar.h"
#include"GrammarPretreatment.h"
#include"TerminationFilter.h"
#include"UnreachabilityFilter.h"
#include"LeftCommonFactorExtractor.h"
#include"LeftRecursionEliminator.h"
#include"StateGraph.h"


static class Intermediary {
public:
	//记录每一个类的名字，在获取每个类的grammar变量时使用
	static enum class ClassName {
		GrammarPretreatment,
		TerminationFilter,
		UnreachabilityFilter,
		LeftCommonFactorExtractor,
		LeftRecursionEliminator,
		First,
		Follow,
		StateGraph,
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
	//获得map集合字符串
	static string getProductionsMap(ClassName classname) {
		return Util::mapValueToString(getGrammar(classname).getProductionsMap(), "\n", "当前所有产生式：\n", "\n\n");
	}


	//GrammarPretreatment

	static void setGrammarFileName(string fileName) {
		GrammarPretreatment::setGrammarFileName(fileName);
	}
	static vector<string> getInputProductions() {
		return GrammarPretreatment::getInputProductions();
	}
	static vector<string> getTooShortProductions() {
		return GrammarPretreatment::getTooShortProductions();
	}
	static vector<string> getBeginningErrorProductions() {
		return GrammarPretreatment::getBeginningErrorProductions();
	}
	static vector<string> getPretreatedProductions() {
		return GrammarPretreatment::getPretreatedProductions();
	}


	//TerminationFilter

	static auto getInterminableProductions() {
		return TerminationFilter::getInterminableProductions();
	}


	//UnreachabilityFilter

	static auto getUnreachableProductions() {
		return UnreachabilityFilter::getUnreachableProductions();
	}
};


