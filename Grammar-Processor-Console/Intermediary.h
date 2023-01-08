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
	//��¼ÿһ��������֣��ڻ�ȡÿ�����grammar����ʱʹ��
	static enum class ClassName {
		GrammarPretreatment,
		TerminationFilter,
		UnreachabilityFilter,
		LeftCommonFactorExtractor,
		LeftRecursionEliminator,
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

};


enum class Intermediary::ClassName;//��¼ÿһ��������֣��ڻ�ȡÿ�����grammar����ʱʹ��
