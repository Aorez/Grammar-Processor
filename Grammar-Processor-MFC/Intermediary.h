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
	//��¼ÿһ��������֣��ڻ�ȡÿ�����grammar����ʱʹ��
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
	//��ȡĳ�����е�grammar����
	//����ͷ�ļ���˳�������GrammarPretreatment��TerminationFilter��ͷ�ļ��Ͳ��û������
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
	//���map�����ַ���
	static string getProductionsMap(ClassName classname) {
		return Util::mapValueToString(getGrammar(classname).getProductionsMap(), "\n", "��ǰ���в���ʽ��\n", "\n\n");
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


