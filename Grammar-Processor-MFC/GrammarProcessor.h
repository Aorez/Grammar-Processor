#pragma once
#include<bits/stdc++.h>


#include"GrammarPretreatment.h"
#include"TerminationFilter.h"
#include"UnreachabilityFilter.h"
#include"LeftCommonFactorExtractor.h"
#include"LeftRecursionEliminator.h"
#include"StateGraph.h"
#include"Util.h"


using namespace std;


//�ܵĹ��ܵ��õľ�̬��
static class GrammarProcessor {
private:
	static StateGraph* state;
public:
	~GrammarProcessor() {
		delete state;
	}

	static void processGrammar() {
		try
		{

		//�ķ�Ԥ����
		GrammarPretreatment::pretreatGrammar();

		//ɾ��������ֹ�Ĳ���ʽ
		TerminationFilter::filterTermination();

		//ɾ�����ɵ���Ĳ���ʽ
		UnreachabilityFilter::filterUnreachability();

		//�����ĸ��
		Alphabet::printAllSymbol();

		//��ȡ������
		LeftCommonFactorExtractor::extractLeftCommonFactor();

		//��ݹ�
		LeftRecursionEliminator::eliminateLeftRecursion();

		//ɾ�����ɵ���
		UnreachabilityFilter::setPrevious(2);
		//UnreachabilityFilter::filterUnreachability();

		//first����follow����
		Grammar& grammar = LeftRecursionEliminator::getGrammar();
		cout << "\nfirstSetMap\n";
		grammar.printFirstSetMap();
		cout << "\nfollowSetMap\n";
		grammar.printFollowSetMap();

		delete state;
		if (!grammar.isLeftLinear() && !grammar.isRightLinear()) {
			state = nullptr;
		}
		else if (grammar.getProductionsMap().empty()) {
			state = nullptr;
		}
		else {
			state = new StateGraph(grammar);
		}

		}
		catch (const std::exception&)
		{
			cout << Util::CATCH_EXCEPTION;
		}
	}


	static StateGraph* getState() {
		return state;
	}
};


StateGraph* GrammarProcessor::state = nullptr;

map<string, Symbol> Alphabet::symbolMap;//�ַ��ļ���
string Alphabet::epsilonLabel = "@";//���ֵķ��ţ���@��
map<Symbol, int> Alphabet::copyCountMap;//��¼A'��A''�ȵ���������ȡ�����ӡ�������ݹ�����Ҫ�½����ţ������Ҫ��¼��������������ظ�

const string Grammar::DEFAULT_START_LABEL = "S";//Ĭ�ϵ��ķ���ʼ����
const string Grammar::DEFAULT_END_LABEL = "#";//Ĭ�ϵ��ķ���������
const string Grammar::productionSplitSymbol = "|";//����ʽ�ķָ�������|��
Symbol Grammar::end = Alphabet::addSymbol(Grammar::DEFAULT_END_LABEL, Symbol::NONTERMINATOR);//�ķ��Ľ�������
Symbol Grammar::start = Alphabet::addSymbol(Grammar::DEFAULT_START_LABEL, Symbol::NONTERMINATOR);//�ķ��Ŀ�ʼ����

vector<string> GrammarPretreatment::inputProductions;//���ļ��ж���Ĳ���ʽ
vector<string> GrammarPretreatment::tooShortProductions;//����ʽ̫�̣�����ʽ����Ҫ�����3
vector<string> GrammarPretreatment::beginningErrorProductions;//����ʽ��ͷ��ʽ���󣬲���ʽ��ͷ��ʽӦ���ơ�A->��
vector<string> GrammarPretreatment::selfProductions;//�Բ���ʽ�ļ��ϣ�����A->A���������ַ���
vector<string> GrammarPretreatment::pretreatedProductions;//��productionInputs����Ԥ�����Ĳ���ʽ���뼯��
const string GrammarPretreatment::LACK_START_ERROR = "ȱ�ٿ�ʼ����S��";//ȱ�ٿ�ʼ���ź�ĳ�����ʾ
string GrammarPretreatment::grammarFileName = "grammar.txt";//�ķ��ļ����ļ���
Grammar GrammarPretreatment::grammar;//Ԥ�������ķ�

enum class Intermediary::ClassName;//��¼ÿһ��������֣��ڻ�ȡÿ�����grammar����ʱʹ��

map<Symbol, vector<Production>> LeftCommonFactorExtractor::deducedProductionsMap;//��������Ƶ���õ������в���ʽ
Grammar LeftCommonFactorExtractor::grammar;//��ȡ�����Ӻ���ķ�
Grammar& LeftCommonFactorExtractor::previousGrammar = UnreachabilityFilter::getGrammar();//��һ�����ܽ��к���ķ�

const string LeftCommonFactorTree::TREE_ROOT_LABEL = "&";//���ĸ��ڵ�ķ��Ŵ�������Ϊ&��

Grammar& LeftRecursionEliminator::previousGrammar = LeftCommonFactorExtractor::getGrammar();//��һ�����ܽ��к���ķ�

Grammar LeftRecursionEliminator::grammar;//������ݹ����ķ�

enum class StateGraph::Selection;

vector<Production> TerminationFilter::interminableProductions;//���治����ֹ�Ĳ���ʽ
vector<string> TerminationFilter::interminableLabels;//���治����ֹ���󲿵ķ���
map<Symbol, pair<int, int>> TerminationFilter::statusMap;//����Ƿ񲻿���ֹʱ����¼��鵽����һ���Ҳ�����һ�����ţ�������״̬��Ϣ
const pair<int, int> TerminationFilter::TERMINATION_UNCHECKED(-1, -1);//��ʾ���󲿻�û�б�����Ƿ񲻿���ֹ
const pair<int, int> TerminationFilter::TERMINABILITY(-2, -2);//��ʾ���󲿱�������ˣ������ǿ�����ֹ��״̬
const pair<int, int> TerminationFilter::INTERMINABILITY(-3, -3);//��ʾ���󲿱�������ˣ��ǲ�����ֹ��
Grammar& TerminationFilter::previousGrammar = GrammarPretreatment::getGrammar();//��һ�����ܽ��к���ķ�
Grammar TerminationFilter::grammar;//ɾ���˲�����ֹ����ķ�

map<Symbol, int> UnreachabilityFilter::recordMap;//��¼�����Ƿ�ɵ���
enum UnreachabilityFilter::reachabilityRecord;//�ڼ�¼�����Ƿ�ɵ����map�����У��ֱ��ʾ�ɵ�����ɵ��δ���
vector<vector<Production>> UnreachabilityFilter::unreachableProductions;//���治�ɵ���Ĳ���ʽ
Grammar UnreachabilityFilter::grammar;//ɾ�����ɵ������ķ�
Grammar& UnreachabilityFilter::previousGrammar = TerminationFilter::getGrammar();//��һ�����ܽ��к���ķ�
Grammar& UnreachabilityFilter::previousGrammar2 = LeftRecursionEliminator::getGrammar();//��һ���ķ�
int UnreachabilityFilter::previous = 1;//��һ���ķ��ı�־


const string Util::CATCH_EXCEPTION = "δ֪����";//�쳣��ʾ
